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

#pragma once

#include "spmod.hpp"

class Native : public INative
{
public:

    Native() = delete;
    ~Native() = default;

    Native(std::string_view owner,
           std::string_view name,
           SPVM_NATIVE_FUNC router,
           SourcePawn::IPluginFunction *func) : m_ownerIdentity(owner),
                                                m_nativeName(name),
                                                m_router(router),
                                                m_func(func)
    {}

    Native(std::string_view owner,
           const sp_nativeinfo_t *native) : m_ownerIdentity(owner),
                                            m_nativeName(native->name),
                                            m_router(native->func),
                                            m_func(nullptr)
    {}

    // INativeInfo

    /**
     * @brief Returns name of the native.
     *
     * @return        Native name.
     */
    const char *getName() const override
    {
        return m_nativeName.c_str();
    }

    /**
     * @brief Returns identity of the owner.
     *
     * @return        Owner identity.
     */
    const char *getOwner() const override
    {
        return m_ownerIdentity.c_str();
    }

    /**
     * @brief Returns plugin function to be executed when native is called.
     *
     * @note          If nullptr then native was added by module.
     *
     * @return        Owner identity.
     */
    SourcePawn::IPluginFunction *getFunc() const override
    {
        return m_func;
    }

    // NativeInfo
    SPVM_NATIVE_FUNC getRouter() const
    {
        return m_router;
    }
    std::string_view getNameCore() const
    {
        return m_nativeName;
    }
    std::string_view getOwnerCore() const
    {
        return m_ownerIdentity;
    }

private:

    std::string m_ownerIdentity;
    std::string m_nativeName;
    SPVM_NATIVE_FUNC m_router;

    // Only valid for fake natives
    SourcePawn::IPluginFunction *m_func; /* Function to execute in plugin */
};

class NativeMngr : public INativeMngr
{
public:
    NativeMngr() = default;
    ~NativeMngr() = default;

    // INativeMngr

    bool addNatives(IModuleInterface *interface) override;
    INative *getNative(const char *name) const override
    {
        return getNativeCore(name).get();
    }

    // NativeMngr

    const auto &getNativesList() const
    {
        return m_natives;
    }

    void clearNatives()
    {
        freeFakeNatives();
        m_natives.clear();
    }

    void freeFakeNatives();
    bool addFakeNative(std::string_view pluginname,
                       std::string_view name,
                       SourcePawn::IPluginFunction *func);
    std::shared_ptr<Native> getNativeCore(std::string_view name) const;

    // For fake natives
    static cell_t fakeNativeRouter(SourcePawn::IPluginContext *ctx,
                                   const cell_t *params,
                                   void *data);

    static inline SourcePawn::IPluginContext *m_callerPlugin;
    static inline cell_t m_callerParams[SP_MAX_EXEC_PARAMS + 1];

private:
    std::unordered_map<std::string, std::shared_ptr<Native>> m_natives;
};