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

bool NativeMngr::addNatives(IModuleInterface *interface)
{
    //TODO: Error reporting?
    if (interface->getInterfaceVersion() > SPMOD_API_VERSION)
        return false;

    const char *moduleName = interface->getName();
    const sp_nativeinfo_t *nativelist = interface->getNatives();
    while (nativelist->name && nativelist->func)
    {
        auto nativeData = std::make_shared<Native>(moduleName, nativelist);

        if (!m_natives.try_emplace(nativelist->name, std::move(nativeData)).second)
            return false;

        nativelist++;
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

    for (size_t i = 0; i <= static_cast<size_t>(params[0]); i++)
        m_callerParams[i] = params[i];

    cell_t result = 0;

    SourcePawn::IPluginFunction *func = native->getFunc();
	func->PushCell(caller->getId());
    func->Execute(&result);

    m_callerPlugin = nullptr;

    return result;
}
