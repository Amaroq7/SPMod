/*
 *  Copyright (C) 2018-2020 SPMod Development Team
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

static qboolean ClientConnect(edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[128])
{
    auto plrMngr = gSPGlobal->getPlayerManager();

    if (!plrMngr->ClientConnect(pEntity, pszName, pszAddress, szRejectReason))
        RETURN_META_VALUE(MRES_SUPERCEDE, FALSE);

    RETURN_META_VALUE(MRES_IGNORED, TRUE);
}

static void ClientCommand(edict_t *pEntity)
{
    {
        int result;
        auto fwdCmd = gSPGlobal->getForwardManager()->getForward(ForwardMngr::FWD_PLAYER_COMMAND);

        if (!fwdCmd)
            RETURN_META(MRES_IGNORED);

        fwdCmd->pushInt(ENTINDEX(pEntity));
        fwdCmd->execFunc(&result);

        if (static_cast<IForward::ReturnValue>(result) == IForward::ReturnValue::Stop)
            RETURN_META(MRES_SUPERCEDE);
    }

    META_RES res = MRES_IGNORED;

    std::string_view strCmd(CMD_ARGV(0));

    if (strCmd == "menuselect")
    {
        res = gSPGlobal->getMenuManager()->ClientCommand(pEntity);
        RETURN_META(res);
    }

    auto cmdMngr = gSPGlobal->getCommandManager();
    RETURN_META(cmdMngr->ClientCommandMeta(pEntity, strCmd));
}

void ClientPutInServer(edict_t *pEntity)
{
    gSPGlobal->getPlayerManager()->ClientPutInServer(pEntity);
}

DLL_FUNCTIONS gDllFunctionTable = {
    nullptr,           // pfnGameInit
    nullptr,           // pfnSpawn
    nullptr,           // pfnThink
    nullptr,           // pfnUse
    nullptr,           // pfnTouch
    nullptr,           // pfnBlocked
    nullptr,           // pfnKeyValue
    nullptr,           // pfnSave
    nullptr,           // pfnRestore
    nullptr,           // pfnSetAbsBox
    nullptr,           // pfnSaveWriteFields
    nullptr,           // pfnSaveReadFields
    nullptr,           // pfnSaveGlobalState
    nullptr,           // pfnRestoreGlobalState
    nullptr,           // pfnResetGlobalState
    ClientConnect,     // pfnClientConnect
    nullptr,           // pfnClientDisconnect
    nullptr,           // pfnClientKill
    ClientPutInServer, // pfnClientPutInServer
    ClientCommand,     // pfnClientCommand
    nullptr,           // pfnClientUserInfoChanged
    nullptr,           // pfnServerActivate
    nullptr,           // pfnServerDeactivate
    nullptr,           // pfnPlayerPreThink
    nullptr,           // pfnPlayerPostThink
    nullptr,           // pfnStartFrame
    nullptr,           // pfnParmsNewLevel
    nullptr,           // pfnParmsChangeLevel
    nullptr,           // pfnGetGameDescription
    nullptr,           // pfnPlayerCustomization
    nullptr,           // pfnSpectatorConnect
    nullptr,           // pfnSpectatorDisconnect
    nullptr,           // pfnSpectatorThink
    nullptr,           // pfnSys_Error
    nullptr,           // pfnPM_Move
    nullptr,           // pfnPM_Init
    nullptr,           // pfnPM_FindTextureType
    nullptr,           // pfnSetupVisibility
    nullptr,           // pfnUpdateClientData
    nullptr,           // pfnAddToFullPack
    nullptr,           // pfnCreateBaseline
    nullptr,           // pfnRegisterEncoders
    nullptr,           // pfnGetWeaponData
    nullptr,           // pfnCmdStart
    nullptr,           // pfnCmdEnd
    nullptr,           // pfnConnectionlessPacket
    nullptr,           // pfnGetHullBounds
    nullptr,           // pfnCreateInstancedBaselines
    nullptr,           // pfnInconsistentFile
    nullptr,           // pfnAllowLagCompensation
};

static void ServerActivatePost(edict_t *pEdictList, int edictCount [[maybe_unused]], int clientMax)
{
    gSPGlobal->clearEdicts();
    gSPGlobal->getPlayerManager()->ServerActivatePost(pEdictList, clientMax);

    auto fwdMngr = gSPGlobal->getForwardManager();
    fwdMngr->addDefaultsForwards();

    gSPGlobal->loadExts();
    gSPGlobal->allowPrecacheForPlugins(true);

    for (auto &interface : gSPGlobal->getAdaptersInterfaces())
    {
        interface.second->getPluginMngr()->loadPlugins();
    }

    // Allow plugins to add their natives
    fwdMngr->getForward(ForwardMngr::FWD_PLUGIN_NATIVES)->execFunc(nullptr);

    for (auto &interface : gSPGlobal->getAdaptersInterfaces())
    {
        interface.second->getPluginMngr()->bindPluginsNatives();
    }

    fwdMngr->getForward(ForwardMngr::FWD_PLUGIN_INIT)->execFunc(nullptr);
    fwdMngr->getForward(ForwardMngr::FWD_PLUGINS_LOADED)->execFunc(nullptr);

    gSPGlobal->allowPrecacheForPlugins(false);

    installRehldsHooks();
}

static void ServerDeactivatePost()
{
    auto fwdMngr = gSPGlobal->getForwardManager();
    fwdMngr->getForward(ForwardMngr::FWD_PLUGIN_END)->execFunc(nullptr);

    for (auto &interface : gSPGlobal->getAdaptersInterfaces())
    {
        interface.second->getPluginMngr()->unloadPlugins();
    }

    fwdMngr->clearForwards();
    gSPGlobal->getTimerManager()->clearTimers();
    gSPGlobal->getCommandManager()->clearCommands();
    gSPGlobal->getCvarManager()->clearCvarsCallback();
    gSPGlobal->getMenuManager()->clearMenus();
    gSPGlobal->getMessageManager()->clearMessages();
    gSPGlobal->getNativeProxy()->clearNatives();
    gSPGlobal->getPlayerManager()->ServerDeactivatePost();

    gSPGlobal->unloadExts();
    uninstallRehldsHooks();
}

static void GameInitPost()
{
    REG_SVR_COMMAND("spmod", CommandMngr::SPModInfoCommand);
}

static qboolean ClientConnectPost(edict_t *pEntity,
                                  const char *pszName,
                                  const char *pszAddress,
                                  char szRejectReason [[maybe_unused]][128])
{
    gSPGlobal->getPlayerManager()->ClientConnectPost(pEntity, pszName, pszAddress);

    RETURN_META_VALUE(MRES_IGNORED, TRUE);
}

static void ClientPutInServerPost(edict_t *pEntity)
{
    gSPGlobal->getPlayerManager()->ClientPutInServerPost(pEntity);
}

static void ClientUserInfoChangedPost(edict_t *pEntity, char *infobuffer)
{
    gSPGlobal->getPlayerManager()->ClientUserInfoChangedPost(pEntity, infobuffer);
}

static void StartFramePost()
{
    gSPGlobal->getPlayerManager()->StartFramePost();

    if (TimerMngr::m_nextExecution <= gpGlobals->time)
    {
        TimerMngr::m_nextExecution = gpGlobals->time + 0.1f;
        gSPGlobal->getTimerManager()->execTimers(gpGlobals->time);
    }

    RETURN_META(MRES_IGNORED);
}

DLL_FUNCTIONS gDllFunctionTablePost = {
    GameInitPost,              // pfnGameInit
    nullptr,                   // pfnSpawn
    nullptr,                   // pfnThink
    nullptr,                   // pfnUse
    nullptr,                   // pfnTouch
    nullptr,                   // pfnBlocked
    nullptr,                   // pfnKeyValue
    nullptr,                   // pfnSave
    nullptr,                   // pfnRestore
    nullptr,                   // pfnSetAbsBox
    nullptr,                   // pfnSaveWriteFields
    nullptr,                   // pfnSaveReadFields
    nullptr,                   // pfnSaveGlobalState
    nullptr,                   // pfnRestoreGlobalState
    nullptr,                   // pfnResetGlobalState
    ClientConnectPost,         // pfnClientConnect
    nullptr,                   // pfnClientDisconnect
    nullptr,                   // pfnClientKill
    ClientPutInServerPost,     // pfnClientPutInServer
    nullptr,                   // pfnClientCommand
    ClientUserInfoChangedPost, // pfnClientUserInfoChanged
    ServerActivatePost,        // pfnServerActivate
    ServerDeactivatePost,      // pfnServerDeactivate
    nullptr,                   // pfnPlayerPreThink
    nullptr,                   // pfnPlayerPostThink
    StartFramePost,            // pfnStartFrame
    nullptr,                   // pfnParmsNewLevel
    nullptr,                   // pfnParmsChangeLevel
    nullptr,                   // pfnGetGameDescription
    nullptr,                   // pfnPlayerCustomization
    nullptr,                   // pfnSpectatorConnect
    nullptr,                   // pfnSpectatorDisconnect
    nullptr,                   // pfnSpectatorThink
    nullptr,                   // pfnSys_Error
    nullptr,                   // pfnPM_Move
    nullptr,                   // pfnPM_Init
    nullptr,                   // pfnPM_FindTextureType
    nullptr,                   // pfnSetupVisibility
    nullptr,                   // pfnUpdateClientData
    nullptr,                   // pfnAddToFullPack
    nullptr,                   // pfnCreateBaseline
    nullptr,                   // pfnRegisterEncoders
    nullptr,                   // pfnGetWeaponData
    nullptr,                   // pfnCmdStart
    nullptr,                   // pfnCmdEnd
    nullptr,                   // pfnConnectionlessPacket
    nullptr,                   // pfnGetHullBounds
    nullptr,                   // pfnCreateInstancedBaselines
    nullptr,                   // pfnInconsistentFile
    nullptr,                   // pfnAllowLagCompensation
};

NEW_DLL_FUNCTIONS gNewDllFunctionTable = {
    nullptr, //! pfnOnFreeEntPrivateData()	Called right before the object's memory is freed.  Calls its destructor.
    nullptr, //! pfnGameShutdown()
    nullptr, //! pfnShouldCollide()
    nullptr, //! pfnCvarValue()
    nullptr, //! pfnCvarValue2()
};

void OnFreeEntPrivateDataPost(edict_t *pEnt)
{
    gSPGlobal->removeEdict(pEnt);
}

NEW_DLL_FUNCTIONS gNewDllFunctionTablePost = {
    OnFreeEntPrivateDataPost, //! pfnOnFreeEntPrivateData()	Called right before the object's memory is freed.  Calls its
                              //! destructor.
    nullptr,                  //! pfnGameShutdown()
    nullptr,                  //! pfnShouldCollide()
    nullptr,                  //! pfnCvarValue()
    nullptr,                  //! pfnCvarValue2()
};

C_DLLEXPORT int GetEntityAPI2(DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion)
{
    if (!pFunctionTable)
        return 0;

    if (*interfaceVersion != INTERFACE_VERSION)
    {
        *interfaceVersion = INTERFACE_VERSION;
        return 0;
    }

    memcpy(pFunctionTable, &gDllFunctionTable, sizeof(DLL_FUNCTIONS));
    return 1;
}

C_DLLEXPORT int GetEntityAPI2_Post(DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion)
{
    if (!pFunctionTable)
        return 0;

    if (*interfaceVersion != INTERFACE_VERSION)
    {
        *interfaceVersion = INTERFACE_VERSION;
        return 0;
    }

    memcpy(pFunctionTable, &gDllFunctionTablePost, sizeof(DLL_FUNCTIONS));
    return 1;
}

C_DLLEXPORT int GetNewDLLFunctions(NEW_DLL_FUNCTIONS *pNewFunctionTable, int *interfaceVersion)
{
    if (!pNewFunctionTable)
        return 0;

    if (*interfaceVersion != NEW_DLL_FUNCTIONS_VERSION)
    {
        *interfaceVersion = NEW_DLL_FUNCTIONS_VERSION;
        return 0;
    }

    memcpy(pNewFunctionTable, &gNewDllFunctionTable, sizeof(NEW_DLL_FUNCTIONS));
    return 1;
}

C_DLLEXPORT int GetNewDLLFunctions_Post(NEW_DLL_FUNCTIONS *pNewFunctionTable, int *interfaceVersion)
{
    if (!pNewFunctionTable)
        return 0;

    if (*interfaceVersion != NEW_DLL_FUNCTIONS_VERSION)
    {
        *interfaceVersion = NEW_DLL_FUNCTIONS_VERSION;
        return 0;
    }

    memcpy(pNewFunctionTable, &gNewDllFunctionTablePost, sizeof(NEW_DLL_FUNCTIONS));
    return 1;
}
