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

static qboolean ClientConnect(edict_t *pEntity,
                              const char *pszName,
                              const char *pszAddress,
                              char szRejectReason[128])
{
    using sflags = IForward::StringFlags;
    using def = ForwardMngr::FwdDefault;

    const std::unique_ptr<PlayerMngr> &plrMngr = gSPGlobal->getPlayerManagerCore();

    if (!plrMngr->ClientConnect(pEntity, pszName, pszAddress, szRejectReason))
        RETURN_META_VALUE(MRES_SUPERCEDE, FALSE);

    IForward::ReturnValue result;
    std::shared_ptr<Forward> forward = gSPGlobal->getForwardManagerCore()->getDefaultForward(def::ClientConnect);

    forward->pushInt(plrMngr->getPlayerCore(pEntity)->getIndex());
    forward->pushString(pszName);
    forward->pushString(pszAddress);
    forward->pushStringEx(szRejectReason, 128, sflags::Utf8 | sflags::Copy, true);
    forward->execFunc(&result);

    if (result == IForward::ReturnValue::PluginStop)
        RETURN_META_VALUE(MRES_SUPERCEDE, FALSE);

    RETURN_META_VALUE(MRES_IGNORED, TRUE);
}

static void ClientCommand(edict_t *pEntity)
{
    using def = ForwardMngr::FwdDefault;

    {
        IForward::ReturnValue result;
        std::shared_ptr<Forward> fwdCmd = gSPGlobal->getForwardManagerCore()->getDefaultForward(def::ClientCommmand);

        if (!fwdCmd)
            RETURN_META(MRES_IGNORED);

        fwdCmd->pushInt(ENTINDEX(pEntity));
        fwdCmd->execFunc(&result);

        if (result == IForward::ReturnValue::PluginStop)
            RETURN_META(MRES_SUPERCEDE);
    }

    META_RES res = MRES_IGNORED;

    std::string strCmd(CMD_ARGV(0));

    const std::unique_ptr<CommandMngr> &cmdMngr = gSPGlobal->getCommandManagerCore();
    if (cmdMngr->getCommandsNum(CmdType::Client))
    {
        if (!strCmd.compare("say") || !strCmd.compare("say_team"))
        {
            strCmd += ' ';
            strCmd += CMD_ARGV(1);
        }

        for (const auto &cmd : cmdMngr->getCommandList(CmdType::Client))
        {
            std::regex cmdToMatch(cmd->getCmd().data());
            if (std::regex_search(strCmd, cmdToMatch) && cmd->hasAccess())
            {
                IForward::ReturnValue result;
                /*SourcePawn::IPluginFunction *func = cmd->getFunc();
                func->PushCell(ENTINDEX(pEntity));
                func->PushCell(cmd->getId());
                func->Execute(&result);*/

                if (result == IForward::ReturnValue::PluginStop || result == IForward::ReturnValue::PluginHandled)
                {
                    res = MRES_SUPERCEDE;
                    if (result == IForward::ReturnValue::PluginStop)
                        break;
                }
            }
        }
    }

    if (!strCmd.compare("menuselect"))
    {
        res = gSPGlobal->getMenuManagerCore()->ClientCommand(pEntity);
    }

    RETURN_META(res);
}

DLL_FUNCTIONS gDllFunctionTable =
{
    nullptr,					// pfnGameInit
    nullptr,               		// pfnSpawn
    nullptr,					// pfnThink
    nullptr,					// pfnUse
    nullptr,					// pfnTouch
    nullptr,					// pfnBlocked
    nullptr,					// pfnKeyValue
    nullptr,					// pfnSave
    nullptr,					// pfnRestore
    nullptr,					// pfnSetAbsBox
    nullptr,					// pfnSaveWriteFields
    nullptr,					// pfnSaveReadFields
    nullptr,					// pfnSaveGlobalState
    nullptr,					// pfnRestoreGlobalState
    nullptr,					// pfnResetGlobalState
    ClientConnect,              // pfnClientConnect
    nullptr,					// pfnClientDisconnect
    nullptr,					// pfnClientKill
    nullptr,					// pfnClientPutInServer
    ClientCommand,              // pfnClientCommand
    nullptr,					// pfnClientUserInfoChanged
    nullptr,                    // pfnServerActivate
    nullptr,	                // pfnServerDeactivate
    nullptr,					// pfnPlayerPreThink
    nullptr,					// pfnPlayerPostThink
    nullptr,					// pfnStartFrame
    nullptr,					// pfnParmsNewLevel
    nullptr,					// pfnParmsChangeLevel
    nullptr,					// pfnGetGameDescription
    nullptr,					// pfnPlayerCustomization
    nullptr,					// pfnSpectatorConnect
    nullptr,					// pfnSpectatorDisconnect
    nullptr,					// pfnSpectatorThink
    nullptr,					// pfnSys_Error
    nullptr,					// pfnPM_Move
    nullptr,					// pfnPM_Init
    nullptr,					// pfnPM_FindTextureType
    nullptr,					// pfnSetupVisibility
    nullptr,					// pfnUpdateClientData
    nullptr,					// pfnAddToFullPack
    nullptr,					// pfnCreateBaseline
    nullptr,					// pfnRegisterEncoders
    nullptr,					// pfnGetWeaponData
    nullptr,					// pfnCmdStart
    nullptr,					// pfnCmdEnd
    nullptr,					// pfnConnectionlessPacket
    nullptr,					// pfnGetHullBounds
    nullptr,					// pfnCreateInstancedBaselines
    nullptr,					// pfnInconsistentFile
    nullptr,					// pfnAllowLagCompensation
};

static void ServerActivatePost(edict_t *pEdictList,
                               int edictCount [[maybe_unused]],
                               int clientMax)
{
    gSPGlobal->getPlayerManagerCore()->ServerActivatePost(pEdictList, clientMax);

    gSPGlobal->getForwardManagerCore()->addDefaultsForwards();

    #error Execute PluginMngr in exts

    gSPGlobal->loadExts();

    /*pluginManager->setPluginPrecache(true);
    pluginManager->loadPlugins();
    pluginManager->setPluginPrecache(false);*/

    #error Tell exts to load plugins

    installRehldsHooks();
}

static void ServerDeactivatePost()
{
    using def = ForwardMngr::FwdDefault;

    const std::unique_ptr<ForwardMngr> &fwdMngr = gSPGlobal->getForwardManagerCore();
    fwdMngr->getDefaultForward(def::PluginEnd)->execFunc(nullptr);

    #error Tell exts to unload plugins

    gSPGlobal->unloadExts();

    gSPGlobal->getTimerManagerCore()->clearTimers();
    gSPGlobal->getCommandManagerCore()->clearCommands();
    gSPGlobal->getCvarManagerCore()->clearCvarsCallback();
    gSPGlobal->getMenuManagerCore()->clearMenus();
    fwdMngr->clearForwards();
    gSPGlobal->getLoggerManagerCore()->getLoggerCore("SPMOD")->resetState();
    uninstallRehldsHooks();
}

static void GameInitPost()
{
    REG_SVR_COMMAND("spmod", SPModInfoCommand);
}

static qboolean ClientConnectPost(edict_t *pEntity,
                                  const char *pszName,
                                  const char *pszAddress,
                                  char szRejectReason [[maybe_unused]] [128])
{
    gSPGlobal->getPlayerManagerCore()->ClientConnectPost(pEntity, pszName, pszAddress);

    // TODO: Add OnClientConnected(int client, const char[] name, const char[] ip) for plugins?

    RETURN_META_VALUE(MRES_IGNORED, TRUE);
}

static void ClientPutInServerPost(edict_t *pEntity)
{
    using def = ForwardMngr::FwdDefault;

    const std::unique_ptr<PlayerMngr> &plrMngr = gSPGlobal->getPlayerManagerCore();
    plrMngr->ClientPutInServerPost(pEntity);

    std::shared_ptr<Forward> forward = gSPGlobal->getForwardManagerCore()->getDefaultForward(def::ClientPutInServer);
    forward->pushInt(plrMngr->getPlayerCore(pEntity)->getIndex());
    forward->execFunc(nullptr);
}

static void ClientUserInfoChangedPost(edict_t *pEntity,
                                      char *infobuffer)
{
    gSPGlobal->getPlayerManagerCore()->ClientUserInfoChangedPost(pEntity, infobuffer);
}

static void StartFramePost()
{
    gSPGlobal->getPlayerManagerCore()->StartFramePost();

    if (TimerMngr::m_nextExecution <= gpGlobals->time)
    {
        TimerMngr::m_nextExecution = gpGlobals->time + 0.1f;
        gSPGlobal->getTimerManagerCore()->execTimers(gpGlobals->time);
    }

    RETURN_META(MRES_IGNORED);
}

DLL_FUNCTIONS gDllFunctionTablePost =
{
    GameInitPost,				// pfnGameInit
    nullptr,					// pfnSpawn
    nullptr,					// pfnThink
    nullptr,					// pfnUse
    nullptr,					// pfnTouch
    nullptr,					// pfnBlocked
    nullptr,					// pfnKeyValue
    nullptr,					// pfnSave
    nullptr,					// pfnRestore
    nullptr,					// pfnSetAbsBox
    nullptr,					// pfnSaveWriteFields
    nullptr,					// pfnSaveReadFields
    nullptr,					// pfnSaveGlobalState
    nullptr,					// pfnRestoreGlobalState
    nullptr,					// pfnResetGlobalState
    ClientConnectPost,          // pfnClientConnect
    nullptr,					// pfnClientDisconnect
    nullptr,					// pfnClientKill
    ClientPutInServerPost,      // pfnClientPutInServer
    nullptr,					// pfnClientCommand
    ClientUserInfoChangedPost,  // pfnClientUserInfoChanged
    ServerActivatePost,			// pfnServerActivate
    ServerDeactivatePost,       // pfnServerDeactivate
    nullptr,					// pfnPlayerPreThink
    nullptr,					// pfnPlayerPostThink
    StartFramePost,             // pfnStartFrame
    nullptr,					// pfnParmsNewLevel
    nullptr,					// pfnParmsChangeLevel
    nullptr,					// pfnGetGameDescription
    nullptr,					// pfnPlayerCustomization
    nullptr,					// pfnSpectatorConnect
    nullptr,					// pfnSpectatorDisconnect
    nullptr,					// pfnSpectatorThink
    nullptr,					// pfnSys_Error
    nullptr,					// pfnPM_Move
    nullptr,					// pfnPM_Init
    nullptr,					// pfnPM_FindTextureType
    nullptr,					// pfnSetupVisibility
    nullptr,					// pfnUpdateClientData
    nullptr,					// pfnAddToFullPack
    nullptr,					// pfnCreateBaseline
    nullptr,					// pfnRegisterEncoders
    nullptr,					// pfnGetWeaponData
    nullptr,					// pfnCmdStart
    nullptr,					// pfnCmdEnd
    nullptr,					// pfnConnectionlessPacket
    nullptr,					// pfnGetHullBounds
    nullptr,					// pfnCreateInstancedBaselines
    nullptr,					// pfnInconsistentFile
    nullptr,					// pfnAllowLagCompensation
};

NEW_DLL_FUNCTIONS gNewDllFunctionTable =
{
    nullptr,					//! pfnOnFreeEntPrivateData()	Called right before the object's memory is freed.  Calls its destructor.
    nullptr,					//! pfnGameShutdown()
    nullptr,					//! pfnShouldCollide()
    nullptr,					//! pfnCvarValue()
    nullptr,					//! pfnCvarValue2()
};

NEW_DLL_FUNCTIONS gNewDllFunctionTablePost =
{
    nullptr,					//! pfnOnFreeEntPrivateData()	Called right before the object's memory is freed.  Calls its destructor.
    nullptr,			        //! pfnGameShutdown()
    nullptr,					//! pfnShouldCollide()
    nullptr,					//! pfnCvarValue()
    nullptr,					//! pfnCvarValue2()
};

C_DLLEXPORT int GetEntityAPI2(DLL_FUNCTIONS *pFunctionTable,
                                int *interfaceVersion)
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

C_DLLEXPORT int GetEntityAPI2_Post(DLL_FUNCTIONS *pFunctionTable,
                                    int *interfaceVersion)
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

C_DLLEXPORT int GetNewDLLFunctions(NEW_DLL_FUNCTIONS *pNewFunctionTable,
                                    int *interfaceVersion)
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

C_DLLEXPORT int GetNewDLLFunctions_Post(NEW_DLL_FUNCTIONS *pNewFunctionTable,
                                            int *interfaceVersion)
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
