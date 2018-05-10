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
    auto fwdPlayerConnect = gSPGlobal->getForwardManagerCore()->findForwardCore("OnClientConnect");
    using sflags = IForward::StringFlags;

    cell_t result;

    fwdPlayerConnect->pushCell(ENTINDEX(pEntity));
    fwdPlayerConnect->pushString(pszName);
    fwdPlayerConnect->pushString(pszAddress);
    fwdPlayerConnect->pushStringEx(szRejectReason, 128, sflags::Utf8 | sflags::Copy, true);
    fwdPlayerConnect->execFunc(&result);

    if (result == IForward::ReturnValue::PluginStop)
        RETURN_META_VALUE(MRES_SUPERCEDE, FALSE);

    RETURN_META_VALUE(MRES_IGNORED, TRUE);
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
	nullptr,					// pfnClientCommand
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
    auto &pluginManager = gSPGlobal->getPluginManagerCore();
    pluginManager->setPluginPrecache(true);
    pluginManager->loadPlugins();
    pluginManager->setPluginPrecache(false);
}

static void ServerDeactivatePost()
{
    const std::unique_ptr<ForwardMngr> &fwdManager = gSPGlobal->getForwardManagerCore();
    fwdManager->findForwardCore("OnPluginEnd")->execFunc(nullptr);

    gSPGlobal->getPluginManagerCore()->detachPlugins();
    gSPGlobal->getForwardManagerCore()->clearNonDefaults();
    gSPGlobal->getLoggerCore()->resetErrorState();
}

static void GameInitPost()
{
    REG_SVR_COMMAND("spmod", SPModInfoCommand);
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
	nullptr,					// pfnClientPutInServer
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

	if (*interfaceVersion != INTERFACE_VERSION) {
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

	if (*interfaceVersion != INTERFACE_VERSION) {
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

	if (*interfaceVersion != NEW_DLL_FUNCTIONS_VERSION) {
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

	if (*interfaceVersion != NEW_DLL_FUNCTIONS_VERSION) {
		*interfaceVersion = NEW_DLL_FUNCTIONS_VERSION;
		return 0;
	}

	memcpy(pNewFunctionTable, &gNewDllFunctionTablePost, sizeof(NEW_DLL_FUNCTIONS));
	return 1;
}
