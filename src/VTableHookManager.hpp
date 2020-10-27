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

#pragma once

#include "spmod.hpp"

class VTableHook : public IVTableHook
{
public:
    VTableHook(std::intptr_t vtable, std::intptr_t entry, std::intptr_t hook, const std::vector<Callback> &callbacks);
    ~VTableHook();

    const std::optional<Param> &getReturn() const override;
    const std::optional<Param> &getOriginalReturn() const override;
    std::vector<Param> &getParams() override;
    void setReturn(Param retVal) override;

    Return execCallbacks(bool post = false);

    template<typename ReturnType = void, typename... Args>
    ReturnType execOriginalFunc(void *instance, Args... args)
    {
#if defined SP_POSIX
        if constexpr (std::is_same_v<ReturnType, void>)
        {
            reinterpret_cast<ReturnType (*)(void *, Args...)>(m_original)(instance, args...);
        }
        else
        {
            ReturnType result = reinterpret_cast<ReturnType (*)(void *, Args...)>(m_original)(instance, args...);
            if (m_originalReturn.size())
            {
                m_originalReturn.top() = result;
            }

            return result;
        }
#else
        if constexpr (std::is_same_v<ReturnType, void>)
        {
            reinterpret_cast<ReturnType(__fastcall *)(void *, int, Args...)>(m_original)(instance, 0, args...);
        }
        else
        {
            ReturnType result =
                reinterpret_cast<ReturnType(__fastcall *)(void *, int, Args...)>(m_original)(instance, 0, args...);

            if (m_originalReturn.size())
            {
                m_originalReturn.top() = result;
            }

            return result;
        }
#endif
    }

    const std::vector<Param> &packParams()
    {
        return m_params.top();
    }

    template<typename T, typename... Args, typename = std::enable_if_t<std::is_convertible_v<T, Param>>>
    const std::vector<Param> &packParams(T arg, Args... args)
    {
        m_params.top().emplace_back(arg);
        return packParams(args...);
    }

    void prepareForCall()
    {
        m_params.emplace();
        m_status.emplace();
        m_originalReturn.emplace();
        m_return.emplace();
    }

    void cleanup()
    {
        m_params.pop();
        m_status.pop();
        m_originalReturn.pop();
        m_return.pop();
    }

    std::intptr_t getEntryFunc() const;
    std::intptr_t getVTable() const;

private:
    std::intptr_t m_vtable;
    std::intptr_t m_entry;
    std::intptr_t m_original;

    const std::vector<Callback> &m_callbacks;

    std::stack<std::vector<Param>> m_params;
    std::stack<Return> m_status;
    std::stack<std::optional<Param>> m_originalReturn;
    std::stack<std::optional<Param>> m_return;
};

class VTableHookManager : public IVTableHookManager
{
public:
    VTableHookManager() = delete;
    VTableHookManager(const fs::path &configsDir, ModType modType);

    VTableHook *registerHook(vFuncType vFunc, std::string_view classname) override;
    VTableHook *getHook(vFuncType funcType) const;

    void addCallback(VTableHook::Callback callback) override;
    std::uint32_t getPevOffset() const;

    void clearHooks();
    void ServerDeactivatePost();

private:
    VTableHook *_getHook(std::intptr_t vtable, std::intptr_t entryFunc);

private:
    std::unordered_map<vFuncType, std::unique_ptr<VTableHook>> m_hooks;
    std::unordered_map<std::string, std::uint32_t> m_vTableOffsets;
    std::vector<VTableHook::Callback> m_callbacks;
};