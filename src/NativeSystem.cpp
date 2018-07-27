/*  SPMod - SourcePawn Scripting Engine for Half-Life
 *  Copyright (C) 2018  SPMod Development Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "spmod.hpp"

Native::Native(std::string_view owner,
               std::string_view name,
               SPVM_NATIVE_FUNC router,
               SourcePawn::IPluginFunction *func) : m_ownerIdentity(owner),
                                                    m_nativeName(name),
                                                    m_router(router),
                                                    m_func(func)
{}

Native::Native(std::string_view owner,
               const sp_nativeinfo_t *native) : m_ownerIdentity(owner),
                                                m_nativeName(native->name),
                                                m_router(native->func),
                                                m_func(nullptr)
{}

const char *Native::getName() const
{
    return m_nativeName.c_str();
}

const char *Native::getOwner() const
{
    return m_ownerIdentity.c_str();
}

SourcePawn::IPluginFunction *Native::getFunc() const
{
    return m_func;
}

SPVM_NATIVE_FUNC Native::getRouter() const
{
    return m_router;
}

std::string_view Native::getNameCore() const
{
    return m_nativeName;
}

std::string_view Native::getOwnerCore() const
{
    return m_ownerIdentity;
}

void NativeMngr::freeFakeNatives()
{
    SourcePawn::ISourcePawnEngine2 *spAPIv2 = gSPGlobal->getSPEnvironment()->APIv2();

    for(auto pair = m_natives.begin(); pair != m_natives.end(); )
    {
        if (!pair->second->getFunc())
        {
            pair++;
            continue;
        }

        spAPIv2->DestroyFakeNative(pair->second->getRouter());
        pair = m_natives.erase(pair);
    }

}

std::shared_ptr<Native> NativeMngr::getNativeCore(std::string_view name) const
{
    auto pair = m_natives.find(name.data());
    
    if (pair != m_natives.end())
        return pair->second;
    
    return nullptr;
}

bool NativeMngr::addNatives(IModuleInterface *interface, const sp_nativeinfo_t *nativeslist)
{
    //TODO: Error reporting?
    if (interface->getInterfaceVersion() > SPMOD_API_VERSION)
        return false;

    const char *moduleName = interface->getName();
    while (nativeslist->name && nativeslist->func)
    {
        auto nativeData = std::make_shared<Native>(moduleName, nativeslist);

        if (!m_natives.try_emplace(nativeslist->name, std::move(nativeData)).second)
            return false;

        nativeslist++;
    }

    return true;
}

bool NativeMngr::addFakeNative(std::string_view pluginname,
                               std::string_view name,
                               SourcePawn::IPluginFunction *func)
{
    SourcePawn::ISourcePawnEngine2 *spAPIv2 = gSPGlobal->getSPEnvironment()->APIv2();
    auto callbackData = reinterpret_cast<void *>(const_cast<char *>(name.data()));
    SPVM_NATIVE_FUNC router = spAPIv2->CreateFakeNative(fakeNativeRouter, callbackData);

    if (!m_natives.try_emplace(name.data(), std::make_shared<Native>(pluginname, name, router, func)).second)
    {
        spAPIv2->DestroyFakeNative(router);
        return false;
    }

    return true;
}

cell_t NativeMngr::fakeNativeRouter(SourcePawn::IPluginContext *ctx,
                                    const cell_t *params,
                                    void *data)
{
    if (params[0] > SP_MAX_EXEC_PARAMS)
    {
        ctx->ReportError("Too many parameters passed to native! %d (max: %d)", params[0], SP_MAX_EXEC_PARAMS);
        return 0;
    }
    // TODO: Support later?
    if (m_callerPlugin)
    {
        ctx->ReportError("Cannot call another plugin native in native callback!");
        return 0;
    }

    const std::unique_ptr<NativeMngr> &nativeMngr = gSPGlobal->getNativeManagerCore();
    std::shared_ptr<Native> native = nativeMngr->getNativeCore(reinterpret_cast<const char *>(data));

    m_callerPlugin = ctx;
    std::shared_ptr<Plugin> caller = gSPGlobal->getPluginManagerCore()->getPluginCore(ctx);

    for (std::size_t i = 0; i <= static_cast<std::size_t>(params[0]); i++)
        m_callerParams[i] = params[i];

    cell_t result = 0;

    SourcePawn::IPluginFunction *func = native->getFunc();
    func->PushCell(caller->getId());
    func->Execute(&result);

    m_callerPlugin = nullptr;

    return result;
}

INative *NativeMngr::getNative(const char *name) const
{
    return getNativeCore(name).get();
}

void NativeMngr::clearNatives()
{
    freeFakeNatives();
    m_natives.clear();
}
