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
    std::shared_ptr<Forward> forward = gSPGlobal->getForwardManagerCore()->getDefaultForward(def::ClientConnect);

    cell_t result;

    forward->pushCell(ENTINDEX(pEntity));
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
    META_RES res = MRES_IGNORED;

    const std::unique_ptr<CommandMngr> &cmdMngr = gSPGlobal->getCommandManagerCore();
    if (cmdMngr->getCommandsNum(CmdType::Client))
    {
        std::string strCmd(CMD_ARGV(0));

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
                cell_t result;
                SourcePawn::IPluginFunction *func = cmd->getFunc();
                func->PushCell(ENTINDEX(pEntity));
                func->PushCell(cmd->getId());
                func->Execute(&result);

                if (result == IForward::ReturnValue::PluginStop)
                    res = MRES_SUPERCEDE;

                break;
            }
        }
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

static void ServerActivatePost(edict_t *pEdictList [[maybe_unused]],
                               int edictCount [[maybe_unused]],
                               int clientMax [[maybe_unused]])
{
    gSPGlobal->getForwardManagerCore()->addDefaultsForwards();

    const std::unique_ptr<PluginMngr> &pluginManager = gSPGlobal->getPluginManagerCore();

    pluginManager->setPluginPrecache(true);
    pluginManager->loadPlugins();
    pluginManager->setPluginPrecache(false);
}

static void ServerDeactivatePost()
{
    using def = ForwardMngr::FwdDefault;

    const std::unique_ptr<ForwardMngr> &fwdMngr = gSPGlobal->getForwardManagerCore();

    fwdMngr->getDefaultForward(def::PluginEnd)->execFunc(nullptr);

    gSPGlobal->getPluginManagerCore()->clearPlugins();
    gSPGlobal->getCommandManagerCore()->clearCommands();
    fwdMngr->clearForwards();
    gSPGlobal->getLoggerCore()->resetErrorState();
    gSPGlobal->getNativeManagerCore()->freeFakeNatives();
}

static void GameInitPost()
{
    REG_SVR_COMMAND("spmod", SPModInfoCommand);
}

static void ClientPutInServerPost(edict_t *pEntity)
{
    using def = ForwardMngr::FwdDefault;

    std::shared_ptr<Forward> forward = gSPGlobal->getForwardManagerCore()->getDefaultForward(def::ClientPutInServer);
    forward->pushCell(ENTINDEX(pEntity));
    forward->execFunc(nullptr);
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
    nullptr,					// pfnClientConnect
    nullptr,					// pfnClientDisconnect
    nullptr,					// pfnClientKill
    ClientPutInServerPost,      // pfnClientPutInServer
    nullptr,					// pfnClientCommand
    nullptr,					// pfnClientUserInfoChanged
    ServerActivatePost,			// pfnServerActivate
    ServerDeactivatePost,       // pfnServerDeactivate
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

NEW_DLL_FUNCTIONS gNewDllFunctionTable =
{
    nullptr,					//! pfnOnFreeEntPrivateData()	Called right before the object's memory is freed.  Calls its destructor.
    nullptr,					//! pfnGameShutdown()
    nullptr,					//! pfnShouldCollide()
    nullptr,					//! pfnCvarValue()
    nullptr,					//! pfnCvarValue2()
};

void GameShutdownPost()
{
    unintRehldsApi();
    gSPGlobal->getSPEnvironment()->Shutdown();
}

NEW_DLL_FUNCTIONS gNewDllFunctionTablePost =
{
    nullptr,					//! pfnOnFreeEntPrivateData()	Called right before the object's memory is freed.  Calls its destructor.
    GameShutdownPost,			//! pfnGameShutdown()
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
