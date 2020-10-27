/*
 *  Copyright (C) 2020 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  SPMod is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with SPMod.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "spmod.hpp"

namespace
{
    /*
        The MIT License (MIT)

        Copyright (c) 2013 Thordin

        Permission is hereby granted, free of charge, to any person obtaining a copy of
        this software and associated documentation files (the "Software"), to deal in
        the Software without restriction, including without limitation the rights to
        use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
        the Software, and to permit persons to whom the Software is furnished to do so,
        subject to the following conditions:

        The above copyright notice and this permission notice shall be included in all
        copies or substantial portions of the Software.

        THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
        IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
        FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
        COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
        IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
        CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
    */

    std::int32_t vtablehook_unprotect(void *region)
    {
#if defined SP_WINDOWS
        MEMORY_BASIC_INFORMATION mbi;
        VirtualQuery(region, &mbi, sizeof(mbi));
        VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_READWRITE, &mbi.Protect);
        return mbi.Protect;
#else
        static std::int32_t pageSize = sysconf(_SC_PAGE_SIZE);
        mprotect(reinterpret_cast<void *>(reinterpret_cast<std::intptr_t>(region) & ~(pageSize - 1)), pageSize,
                 PROT_READ | PROT_WRITE | PROT_EXEC);
        return PROT_READ | PROT_EXEC;
#endif
    }

    void vtablehook_protect(void *region, int protection)
    {
#if defined SP_WINDOWS
        MEMORY_BASIC_INFORMATION mbi;
        VirtualQuery(region, &mbi, sizeof(mbi));
        VirtualProtect(mbi.BaseAddress, mbi.RegionSize, protection, &mbi.Protect);
#else
        static std::int32_t pageSize = sysconf(_SC_PAGE_SIZE);
        mprotect(reinterpret_cast<void *>(reinterpret_cast<std::intptr_t>(region) & ~(pageSize - 1)), pageSize,
                 protection);
#endif
    }
} // namespace

namespace
{
    std::uint32_t instanceToId(void *instance)
    {
        static std::uint32_t offset = gSPGlobal->getVTableManager()->getPevOffset();
        return ENTINDEX(ENT(*reinterpret_cast<entvars_t **>(reinterpret_cast<std::intptr_t *>(instance) + offset)));
    }
    /* virtual void	Spawn( void ) { return; } */
    void vSpawnHook(
#if defined SP_POSIX
        void *instance
#endif
    )
    {
#if defined SP_WINDOWS
        void *instance;
        __asm
        {
            mov [instance], ecx
        }
#endif
        VTableHook *hook = gSPGlobal->getVTableManager()->getHook(VTableHookManager::vFuncType::Spawn);
        hook->prepareForCall();
        hook->packParams(instanceToId(instance));

        VTableHook::Return retValve = hook->execCallbacks();

        if (retValve != VTableHook::Return::Supercede)
        {
            hook->execOriginalFunc<>(instance);
        }

        hook->execCallbacks(true);
        hook->cleanup();
    }

    /*int TakeDamage( entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType); */
    std::int32_t vTakeDamageHook(
#if defined SP_POSIX
        void *instance,
#endif
        entvars_t *pevInflictor,
        entvars_t *pevAttacker,
        float flDamage,
        std::int32_t bitsDamageType [[maybe_unused]])
    {
#if defined SP_WINDOWS
        void *instance;
        __asm
        {
            mov [instance], ecx
        }
#endif
        VTableHook *hook = gSPGlobal->getVTableManager()->getHook(VTableHookManager::vFuncType::TakeDamage);
        hook->prepareForCall();
        hook->packParams(instanceToId(instance), gSPGlobal->getEngine()->getEntVars(pevInflictor),
                         gSPGlobal->getEngine()->getEntVars(pevAttacker), flDamage, bitsDamageType);

        VTableHook::Return status = hook->execCallbacks();
        if (status == VTableHook::Return::Ignored)
        {
            hook->execOriginalFunc<std::int32_t, entvars_t *, entvars_t *, float, std::int32_t>(
                instance, pevInflictor, pevAttacker, flDamage, bitsDamageType);
        }
        else if (status >= VTableHook::Return::Handled && status <= VTableHook::Return::Override)
        {
            const auto &params = hook->getParams();
            hook->execOriginalFunc<std::int32_t, entvars_t *, entvars_t *, float, std::int32_t>(
                instance, *static_cast<Engine::EntVars *>(std::get<Engine::IEntVars *>(params[1])),
                *static_cast<Engine::EntVars *>(std::get<Engine::IEntVars *>(params[2])), std::get<float>(params[3]),
                std::get<std::int32_t>(params[4]));
        }

        status = hook->execCallbacks(true);

        if (status >= VTableHook::Return::Override && hook->getReturn())
        {
            auto retVal = std::get<std::int32_t>(*hook->getReturn());
            hook->cleanup();
            return retVal;
        }

        if (hook->getOriginalReturn())
        {
            auto retVal = std::get<std::int32_t>(*hook->getOriginalReturn());
            hook->cleanup();
            return retVal;
        }

        // Supercede was returned without return value set
        return 0;
    }

    /*void TraceAttack( entvars_t *pevAttacker, float flDamage, Vector vecDir, TraceResult *ptr, int
     * bitsDamageType); */
    void vTraceAttack(
#if defined SP_POSIX
        void *instance,
#endif
        entvars_t *pevAttacker,
        float flDamage,
        Vector vecDir,
        ::TraceResult *ptr,
        std::int32_t bitsDamageType)
    {
#if defined SP_WINDOWS
        void *instance;
        __asm
        {
            mov [instance], ecx
        }
#endif
        VTableHook *hook = gSPGlobal->getVTableManager()->getHook(VTableHookManager::vFuncType::TraceAttack);
        hook->prepareForCall();
        Engine::TraceResult *tr = gSPGlobal->getEngine()->createTraceResult(ptr);
        hook->packParams(instanceToId(instance), gSPGlobal->getEngine()->getEntVars(pevAttacker), flDamage, &vecDir.x,
                         tr, bitsDamageType);

        VTableHook::Return status = hook->execCallbacks();
        if (status == VTableHook::Return::Ignored)
        {
            hook->execOriginalFunc<void, entvars_t *, float, Vector, ::TraceResult *, std::int32_t>(
                instance, pevAttacker, flDamage, vecDir, ptr, bitsDamageType);
        }
        else if (status >= VTableHook::Return::Handled && status <= VTableHook::Return::Override)
        {
            const auto &params = hook->getParams();

            hook->execOriginalFunc<void, entvars_t *, float, Vector, ::TraceResult *, std::int32_t>(
                instance, *static_cast<Engine::EntVars *>(std::get<Engine::IEntVars *>(params[1])), std::get<float>(params[2]),
                Vector(std::get<float *>(params[3])), *tr, std::get<std::int32_t>(params[5]));
        }

        hook->execCallbacks(true);
        gSPGlobal->getEngine()->freeTraceResult(tr);
        hook->cleanup();
    }

} // namespace

VTableHook::VTableHook(std::intptr_t vtable,
                       std::intptr_t entry,
                       std::intptr_t hook,
                       const std::vector<Callback> &callbacks)
    : m_vtable(vtable), m_entry(entry), m_callbacks(callbacks)
{
    m_original = *(reinterpret_cast<std::intptr_t *>(m_entry));

    int original_protection = vtablehook_unprotect(reinterpret_cast<void *>(m_entry));
    *(reinterpret_cast<std::intptr_t *>(m_entry)) = hook;
    vtablehook_protect(reinterpret_cast<void *>(m_entry), original_protection);
}

VTableHook::~VTableHook()
{
    std::int32_t original_protection = vtablehook_unprotect(reinterpret_cast<void *>(m_entry));
    *(reinterpret_cast<std::intptr_t *>(m_entry)) = m_original;
    vtablehook_protect(reinterpret_cast<void *>(m_entry), original_protection);
}

VTableHook::Return VTableHook::execCallbacks(bool post)
{
    Return returnVal = Return::Ignored;
    for (const auto &callback : m_callbacks)
    {
        if (Return result = callback(this, post); result > returnVal)
        {
            returnVal = result;
        }
    }

    return returnVal;
}

const std::optional<VTableHook::Param> &VTableHook::getReturn() const
{
    if (m_return.size())
    {
        return m_return.top();
    }

    static std::optional<Param> empty;
    return empty;
}

const std::optional<VTableHook::Param> &VTableHook::getOriginalReturn() const
{
    if (m_originalReturn.size())
    {
        return m_originalReturn.top();
    }

    static std::optional<Param> empty;
    return empty;
}

std::vector<VTableHook::Param> &VTableHook::getParams()
{
    if (m_params.size())
    {
        return m_params.top();
    }

    static std::vector<Param> empty;
    return empty;
}

void VTableHook::setReturn(Param retVal)
{
    if (m_return.size())
    {
        m_return.top() = retVal;
    }
}

std::intptr_t VTableHook::getEntryFunc() const
{
    return m_entry;
}

std::intptr_t VTableHook::getVTable() const
{
    return m_vtable;
}

VTableHookManager::VTableHookManager(const fs::path &configsDir, ModType modType)
{
    try
    {
        try
        {
            fs::path virtualOffsetsPath =
                configsDir / "gamedata" / "virtual" / GET_GAME_INFO(PLID, GINFO_NAME) / "offsets.yml";
#if defined SP_POSIX
            YAML::Node rootNode = YAML::LoadFile(virtualOffsetsPath.c_str());
            std::string_view osName("linux");
#else
            YAML::Node rootNode = YAML::LoadFile(virtualOffsetsPath.string().c_str());
            std::string_view osName("windows");
#endif
            for (auto funcIt = rootNode.begin(); funcIt != rootNode.end(); ++funcIt)
            {
                m_vTableOffsets.try_emplace(funcIt->first.as<std::string>(),
                                            funcIt->second[osName.data()].as<std::uint32_t>());
            }
        }
        catch (const YAML::BadFile &e [[maybe_unused]])
        {
            if (modType == ModType::Cstrike || modType == ModType::CZero) //not required since we have regamedll_cs
            {
                return;
            }
            else
            {
                throw;
            }
        }
    }
    catch (const std::exception &e)
    {
        using namespace std::string_literals;
        throw std::runtime_error("Error parsing yaml vtable offsets file: "s + e.what());
    }
}

VTableHook *VTableHookManager::registerHook(VTableHookManager::vFuncType vFunc, std::string_view classname)
{
    std::uint32_t vFuncOffset = 0;
    std::intptr_t hookFunc;
    try
    {
        switch (vFunc)
        {
            case vFuncType::Spawn:
            {
                vFuncOffset = m_vTableOffsets.at("spawn");
                hookFunc = reinterpret_cast<std::intptr_t>(vSpawnHook);
                break;
            }
            case vFuncType::TakeDamage:
            {
                vFuncOffset = m_vTableOffsets.at("takedamage");
                hookFunc = reinterpret_cast<std::intptr_t>(vTakeDamageHook);
                break;
            }
            case vFuncType::TraceAttack:
            {
                vFuncOffset = m_vTableOffsets.at("traceattack");
                hookFunc = reinterpret_cast<std::intptr_t>(vTraceAttack);
                break;
            }
            default:
                return nullptr;
        }
    }
    catch (const std::exception &e  [[maybe_unused]])
    {
        return nullptr;
    }

    edict_t *pEdict = CREATE_ENTITY();

    CALL_GAME_ENTITY(PLID, classname.data(), &pEdict->v);

    if (!pEdict->pvPrivateData)
    {
        REMOVE_ENTITY(pEdict);
        return nullptr;
    }
    std::intptr_t vtable = *(reinterpret_cast<std::intptr_t *>(pEdict->pvPrivateData));

    REMOVE_ENTITY(pEdict);

    if (!reinterpret_cast<void *>(vtable))
    {
        return nullptr;
    }

    std::intptr_t entry = vtable + sizeof(std::intptr_t) * vFuncOffset;

    if (auto hook = _getHook(vtable, entry); hook)
    {
        return hook;
    }

    auto result = m_hooks.emplace(vFunc, std::make_unique<VTableHook>(vtable, entry, hookFunc, m_callbacks));

    return result.first->second.get();
}

VTableHook *VTableHookManager::getHook(vFuncType funcType) const
{
    return m_hooks.at(funcType).get();
}

VTableHook *VTableHookManager::_getHook(std::intptr_t vtable, std::intptr_t entryFunc)
{
    for (auto &[funcType, hook] : m_hooks)
    {
        if (hook->getVTable() == vtable && hook->getEntryFunc() == entryFunc)
        {
            return hook.get();
        }
    }

    return nullptr;
}

std::uint32_t VTableHookManager::getPevOffset() const
{
    return m_vTableOffsets.at("pev");
}

void VTableHookManager::addCallback(VTableHook::Callback callback)
{
    m_callbacks.emplace_back(callback);
}

void VTableHookManager::clearHooks()
{
    m_hooks.clear();
    m_callbacks.clear();
}

void VTableHookManager::ServerDeactivatePost()
{
    clearHooks();
}