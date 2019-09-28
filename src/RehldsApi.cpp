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

IRehldsApi *gRehldsApi;
const RehldsFuncs_t *gRehldsFuncs;
IRehldsHookchains *gRehldsHookchains;
IRehldsServerStatic *gRehldsServerStatic;
IRehldsServerData *gRehldsServerData;

static void SV_DropClientHook(IRehldsHook_SV_DropClient *chain,
                              IGameClient *client,
                              bool crash,
                              const char *string)
{
    using def = ForwardMngr::FwdDefault;

    const std::unique_ptr<PlayerMngr> &plrMngr = gSPGlobal->getPlayerManagerCore();
    std::shared_ptr<Player> plr = plrMngr->getPlayerCore(client->GetEdict());

    std::shared_ptr<Forward> forward = gSPGlobal->getForwardManagerCore()->getDefaultForward(def::ClientDisconnect);
    forward->pushInt(plr->getIndex());
    forward->pushInt(crash);
    forward->pushString(string);
    forward->execFunc(nullptr);

    chain->callNext(client, crash, string);

    PlayerMngr::m_playersNum--;
    plr->disconnect();

    forward = gSPGlobal->getForwardManagerCore()->getDefaultForward(def::ClientDisconnectPost);
    forward->pushInt(plr->getIndex());
    forward->pushInt(crash);
    forward->pushString(string);
    forward->execFunc(nullptr);
}

static void Cvar_DirectSetHook(IRehldsHook_Cvar_DirectSet *chain,
                               cvar_t *cvar,
                               const char *value)
{
    auto cachedCvar = gSPGlobal->getCvarManagerCore()->findCvarCore(cvar->name, true);
    // If cached cvar is the same, do not update cached value
    if (cachedCvar && cachedCvar->asStringCore().compare(value))
    {
        cachedCvar->setValue(value);
    }

    chain->callNext(cvar, value);
}

static bool _initRehldsApi(CSysModule *module,
                            std::string *error = nullptr)
{
    if (!module)
    {
        if (error)
            *error = "Failed to locate engine module";

        return false;
    }

    CreateInterfaceFn ifaceFactory = Sys_GetFactory(module);
    if (!ifaceFactory)
    {
        if (error)
            *error = "Failed to locate interface factory in engine module";

        return false;
    }

    int retCode = 0;
    gRehldsApi = reinterpret_cast<IRehldsApi *>(ifaceFactory(VREHLDS_HLDS_API_VERSION, &retCode));
    if (!gRehldsApi)
    {
        if (error)
        {
            *error = "Failed to retrieve rehlds api interface, code ";
            *error += std::to_string(retCode);
        }

        return false;
    }

    auto majorVersion = gRehldsApi->GetMajorVersion();
    auto minorVersion = gRehldsApi->GetMinorVersion();

    if (majorVersion != REHLDS_API_VERSION_MAJOR)
    {
        std::stringstream msg;

        msg << "ReHLDS API Major version mismatch; expected " << REHLDS_API_VERSION_MAJOR;
        msg << ", got " << majorVersion;

        if (error)
            *error = msg.str();

        return false;
    }

    if (minorVersion < REHLDS_API_VERSION_MINOR)
    {
        std::stringstream msg;

        msg << "ReHLDS API minor version mismatch; excpected at least " << REHLDS_API_VERSION_MINOR;
        msg << ", got " << minorVersion;

        if (error)
            *error = msg.str();

        return false;
    }

    gRehldsFuncs = gRehldsApi->GetFuncs();
    gRehldsHookchains = gRehldsApi->GetHookchains();
    gRehldsServerStatic = gRehldsApi->GetServerStatic();
    gRehldsServerData = gRehldsApi->GetServerData();

    return true;
}

bool initRehldsApi()
{
    std::string errorMsg;

#if defined SP_POSIX
    CSysModule *engineModule = Sys_LoadModule("engine_i486.so");
    if (!_initRehldsApi(engineModule, &errorMsg))
    {
        gSPGlobal->getLoggerManagerCore()->getLoggerCore("SPMOD")->logToBothCore(LogLevel::Error, errorMsg.c_str());
        return false;
    }
#else
    CSysModule *engineModule = Sys_LoadModule("swds.dll");
    if (!_initRehldsApi(engineModule))
    {
        engineModule = Sys_LoadModule("filesystem_stdio.dll");
        if (!_initRehldsApi(engineModule, &errorMsg))
        {
            gSPGlobal->getLoggerManagerCore()->getLoggerCore("SPMOD")->logToBothCore(LogLevel::Error, errorMsg.c_str());
            return false;
        }
    }
#endif

    return true;
}

void installRehldsHooks()
{
    gRehldsHookchains->SV_DropClient()->registerHook(SV_DropClientHook);
    gRehldsHookchains->Cvar_DirectSet()->registerHook(Cvar_DirectSetHook);
}

void uninstallRehldsHooks()
{
    gRehldsHookchains->SV_DropClient()->unregisterHook(SV_DropClientHook);
    gRehldsHookchains->Cvar_DirectSet()->unregisterHook(Cvar_DirectSetHook);
}
