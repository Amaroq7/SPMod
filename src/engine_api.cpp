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

static void ChangeLevel(const char *s1, const char *s2 [[maybe_unused]])
{
    auto fwdMngr = gSPGlobal->getForwardManager();
    auto fwdMapChange = fwdMngr->getForward(ForwardMngr::FWD_MAP_CHANGE);
    std::int32_t result;

    fwdMapChange->pushString(s1);
    fwdMapChange->execFunc(&result);

    if (result == IForward::ReturnValue::Stop)
        RETURN_META(MRES_SUPERCEDE);

    RETURN_META(MRES_IGNORED);
}

static void
    MessageBegin_Pre(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed)
{
    auto spPlayer = gSPGlobal->getPlayerManager()->getPlayer(ENTINDEX(ed));
    if (msg_type == gmsgShowMenu || msg_type == gmsgVGUIMenu)
    {
        spPlayer->closeMenu();
    }
    
    META_RES ret = MRES_IGNORED;

    ret = gSPGlobal->getMessageManager()->MessageBegin(msg_dest, msg_type, pOrigin, ed);
    
    RETURN_META(ret);
}
static void MessageEnd_Pre(void)
{
    META_RES ret = MRES_IGNORED;

    ret = gSPGlobal->getMessageManager()->MessageEnd();
    
    RETURN_META(ret);
}
static void WriteByte_Pre(int iValue)
{
    META_RES ret = MRES_IGNORED;

    ret = gSPGlobal->getMessageManager()->WriteParam(MsgParamType::Byte, iValue);
    
    RETURN_META(ret);
}
static void WriteChar_Pre(int iValue)
{
    META_RES ret = MRES_IGNORED;

    ret = gSPGlobal->getMessageManager()->WriteParam(MsgParamType::Char, iValue);
    
    RETURN_META(ret);
}
static void WriteShort_Pre(int iValue)
{
    META_RES ret = MRES_IGNORED;

    ret = gSPGlobal->getMessageManager()->WriteParam(MsgParamType::Short, iValue);
    
    RETURN_META(ret);
}
static void WriteLong_Pre(int iValue)
{
    META_RES ret = MRES_IGNORED;

    ret = gSPGlobal->getMessageManager()->WriteParam(MsgParamType::Long, iValue);
    
    RETURN_META(ret);
}
static void WriteAngle_Pre(float flValue)
{
    META_RES ret = MRES_IGNORED;

    ret = gSPGlobal->getMessageManager()->WriteParam(MsgParamType::Angle, flValue);
    
    RETURN_META(ret);
}
static void WriteCoord_Pre(float flValue)
{
    META_RES ret = MRES_IGNORED;

    ret = gSPGlobal->getMessageManager()->WriteParam(MsgParamType::Coord, flValue);
    
    RETURN_META(ret);
}
static void WriteString_Pre(const char *sz)
{
    META_RES ret = MRES_IGNORED;

    ret = gSPGlobal->getMessageManager()->WriteParam(MsgParamType::String, sz);
    
    RETURN_META(ret);
}
static void WriteEntity_Pre(int iValue)
{
    META_RES ret = MRES_IGNORED;

    ret = gSPGlobal->getMessageManager()->WriteParam(MsgParamType::Entity, iValue);
    
    RETURN_META(ret);
}

enginefuncs_t gEngineFunctionsTable =
{
    nullptr,		// pfnPrecacheModel()
    nullptr,		// pfnPrecacheSound()
    nullptr,		// pfnSetModel()
    nullptr,		// pfnModelIndex()
    nullptr,		// pfnModelFrames()
    nullptr,		// pfnSetSize()
    ChangeLevel,    // pfnChangeLevel()
    nullptr,		// pfnGetSpawnParms()
    nullptr,		// pfnSaveSpawnParms()
    nullptr,		// pfnVecToYaw()
    nullptr,		// pfnVecToAngles()
    nullptr,		// pfnMoveToOrigin()
    nullptr,		// pfnChangeYaw()
    nullptr,		// pfnChangePitch()
    nullptr,		// pfnFindEntityByString()
    nullptr,		// pfnGetEntityIllum()
    nullptr,		// pfnFindEntityInSphere()
    nullptr,		// pfnFindClientInPVS()
    nullptr,		// pfnEntitiesInPVS()
    nullptr,		// pfnMakeVectors()
    nullptr,		// pfnAngleVectors()
    nullptr,		// pfnCreateEntity()
    nullptr,		// pfnRemoveEntity()
    nullptr,		// pfnCreateNamedEntity()
    nullptr,		// pfnMakeStatic()
    nullptr,		// pfnEntIsOnFloor()
    nullptr,		// pfnDropToFloor()
    nullptr,		// pfnWalkMove()
    nullptr,		// pfnSetOrigin()
    nullptr,		// pfnEmitSound()
    nullptr,		// pfnEmitAmbientSound()
    nullptr,		// pfnTraceLine()
    nullptr,		// pfnTraceToss()
    nullptr,		// pfnTraceMonsterHull()
    nullptr,		// pfnTraceHull()
    nullptr,		// pfnTraceModel()
    nullptr,		// pfnTraceTexture()
    nullptr,		// pfnTraceSphere()
    nullptr,		// pfnGetAimVector()
    nullptr,		// pfnServerCommand()
    nullptr,		// pfnServerExecute()
    nullptr,		// pfnClientCommand()
    nullptr,		// pfnParticleEffect()
    nullptr,		// pfnLightStyle()
    nullptr,		// pfnDecalIndex()
    nullptr,		// pfnPointContents()
    MessageBegin_Pre,		// pfnMessageBegin()
    MessageEnd_Pre,		// pfnMessageEnd()
    WriteByte_Pre,		// pfnWriteByte()
    WriteChar_Pre,		// pfnWriteChar()
    WriteShort_Pre,		// pfnWriteShort()
    WriteLong_Pre,		// pfnWriteLong()
    WriteAngle_Pre,		// pfnWriteAngle()
    WriteCoord_Pre,		// pfnWriteCoord()
    WriteString_Pre,		// pfnWriteString()
    WriteEntity_Pre,		// pfnWriteEntity()
    nullptr,		// pfnCVarRegister()
    nullptr,		// pfnCVarGetFloat()
    nullptr,		// pfnCVarGetString()
    nullptr,		// pfnCVarSetFloat()
    nullptr,		// pfnCVarSetString()
    nullptr,		// pfnAlertMessage()
    nullptr,		// pfnEngineFprintf()
    nullptr,		// pfnPvAllocEntPrivateData()
    nullptr,		// pfnPvEntPrivateData()
    nullptr,		// pfnFreeEntPrivateData()
    nullptr,		// pfnSzFromIndex()
    nullptr,		// pfnAllocString()
    nullptr,		// pfnGetVarsOfEnt()
    nullptr,		// pfnPEntityOfEntOffset()
    nullptr,		// pfnEntOffsetOfPEntity()
    nullptr,		// pfnIndexOfEdict()
    nullptr,		// pfnPEntityOfEntIndex()
    nullptr,		// pfnFindEntityByVars()
    nullptr,		// pfnGetModelPtr()
    nullptr,		// pfnRegUserMsg()
    nullptr,		// pfnAnimationAutomove()
    nullptr,		// pfnGetBonePosition()
    nullptr,		// pfnFunctionFromName()
    nullptr,		// pfnNameForFunction()
    nullptr,		// pfnClientPrintf()
    nullptr,		// pfnServerPrint()
    nullptr,		// pfnCmd_Args()
    nullptr,		// pfnCmd_Argv()
    nullptr,		// pfnCmd_Argc()
    nullptr,		// pfnGetAttachment()
    nullptr,		// pfnCRC32_Init()
    nullptr,		// pfnCRC32_ProcessBuffer()
    nullptr,		// pfnCRC32_ProcessByte()
    nullptr,		// pfnCRC32_Final()
    nullptr,		// pfnRandomLong()
    nullptr,		// pfnRandomFloat()
    nullptr,		// pfnSetView()
    nullptr,		// pfnTime()
    nullptr,		// pfnCrosshairAngle()
    nullptr,		// pfnLoadFileForMe()
    nullptr,		// pfnFreeFile()
    nullptr,		// pfnEndSection()
    nullptr,		// pfnCompareFileTime()
    nullptr,		// pfnGetGameDir()
    nullptr,		// pfnCvar_RegisterVariable()
    nullptr,		// pfnFadeClientVolume()
    nullptr,		// pfnSetClientMaxspeed()
    nullptr,		// pfnCreateFakeClient()
    nullptr,		// pfnRunPlayerMove()
    nullptr,		// pfnNumberOfEntities()
    nullptr,		// pfnGetInfoKeyBuffer()
    nullptr,		// pfnInfoKeyValue()
    nullptr,		// pfnSetKeyValue()
    nullptr,		// pfnSetClientKeyValue()
    nullptr,		// pfnIsMapValid()
    nullptr,		// pfnStaticDecal()
    nullptr,		// pfnPrecacheGeneric()
    nullptr,		// pfnGetPlayerUserId()
    nullptr,		// pfnBuildSoundMsg()
    nullptr,		// pfnIsDedicatedServer()
    nullptr,		// pfnCVarGetPointer()
    nullptr,		// pfnGetPlayerWONId()
    nullptr,		// pfnInfo_RemoveKey()
    nullptr,		// pfnGetPhysicsKeyValue()
    nullptr,		// pfnSetPhysicsKeyValue()
    nullptr,		// pfnGetPhysicsInfoString()
    nullptr,		// pfnPrecacheEvent()
    nullptr,		// pfnPlaybackEvent()
    nullptr,		// pfnSetFatPVS()
    nullptr,		// pfnSetFatPAS()
    nullptr,		// pfnCheckVisibility()
    nullptr,		// pfnDeltaSetField()
    nullptr,		// pfnDeltaUnsetField()
    nullptr,		// pfnDeltaAddEncoder()
    nullptr,		// pfnGetCurrentPlayer()
    nullptr,		// pfnCanSkipPlayer()
    nullptr,		// pfnDeltaFindField()
    nullptr,		// pfnDeltaSetFieldByIndex()
    nullptr,		// pfnDeltaUnsetFieldByIndex()
    nullptr,		// pfnSetGroupMask()
    nullptr,		// pfnCreateInstancedBaseline()
    nullptr,		// pfnCvar_DirectSet()
    nullptr,		// pfnForceUnmodified()
    nullptr,		// pfnGetPlayerStats()
    nullptr,		// pfnAddServerCommand()

    // Added in SDK 2.2:
    nullptr, // pfnVoice_GetClientListening()
    nullptr, // pfnVoice_SetClientListening()

    // Added for HL 1109 (no SDK update):
    nullptr, // pfnGetPlayerAuthId()

    // Added 2003/11/10 (no SDK update):
    nullptr, // pfnSequenceGet()
    nullptr, // pfnSequencePickSentence()
    nullptr, // pfnGetFileSize()
    nullptr, // pfnGetApproxWavePlayLen()
    nullptr, // pfnIsCareerMatch()
    nullptr, // pfnGetLocalizedStringLength()
    nullptr, // pfnRegisterTutorMessageShown()
    nullptr, // pfnGetTimesTutorMessageShown()
    nullptr, // pfnProcessTutorMessageDecayBuffer()
    nullptr, // pfnConstructTutorMessageDecayBuffer()
    nullptr, // pfnResetTutorMessageDecayData()

    // Added Added 2005-08-11 (no SDK update)
    nullptr, // pfnQueryClientCvarValue()
    // Added Added 2005-11-22 (no SDK update)
    nullptr, // pfnQueryClientCvarValue2()
    // Added 2009-06-17 (no SDK update)
    nullptr, // pfnEngCheckParm()
};

static int RegUserMsg_Post(const char *pszName, int iSize [[maybe_unused]])
{
    if (!strcmp(pszName, "ShowMenu"))
    {
        gmsgShowMenu = META_RESULT_ORIG_RET(int);
    }
    else if (!strcmp(pszName, "VGUIMenu"))
    {
        gmsgVGUIMenu = META_RESULT_ORIG_RET(int);
    }
    RETURN_META_VALUE(MRES_IGNORED, 0);
}

enginefuncs_t gEngineFunctionsTablePost = {
    nullptr,         // pfnPrecacheModel()
    nullptr,         // pfnPrecacheSound()
    nullptr,         // pfnSetModel()
    nullptr,         // pfnModelIndex()
    nullptr,         // pfnModelFrames()
    nullptr,         // pfnSetSize()
    nullptr,         // pfnChangeLevel()
    nullptr,         // pfnGetSpawnParms()
    nullptr,         // pfnSaveSpawnParms()
    nullptr,         // pfnVecToYaw()
    nullptr,         // pfnVecToAngles()
    nullptr,         // pfnMoveToOrigin()
    nullptr,         // pfnChangeYaw()
    nullptr,         // pfnChangePitch()
    nullptr,         // pfnFindEntityByString()
    nullptr,         // pfnGetEntityIllum()
    nullptr,         // pfnFindEntityInSphere()
    nullptr,         // pfnFindClientInPVS()
    nullptr,         // pfnEntitiesInPVS()
    nullptr,         // pfnMakeVectors()
    nullptr,         // pfnAngleVectors()
    nullptr,         // pfnCreateEntity()
    nullptr,         // pfnRemoveEntity()
    nullptr,         // pfnCreateNamedEntity()
    nullptr,         // pfnMakeStatic()
    nullptr,         // pfnEntIsOnFloor()
    nullptr,         // pfnDropToFloor()
    nullptr,         // pfnWalkMove()
    nullptr,         // pfnSetOrigin()
    nullptr,         // pfnEmitSound()
    nullptr,         // pfnEmitAmbientSound()
    nullptr,         // pfnTraceLine()
    nullptr,         // pfnTraceToss()
    nullptr,         // pfnTraceMonsterHull()
    nullptr,         // pfnTraceHull()
    nullptr,         // pfnTraceModel()
    nullptr,         // pfnTraceTexture()
    nullptr,         // pfnTraceSphere()
    nullptr,         // pfnGetAimVector()
    nullptr,         // pfnServerCommand()
    nullptr,         // pfnServerExecute()
    nullptr,         // pfnClientCommand()
    nullptr,         // pfnParticleEffect()
    nullptr,         // pfnLightStyle()
    nullptr,         // pfnDecalIndex()
    nullptr,         // pfnPointContents()
    nullptr,         // pfnMessageBegin()
    nullptr,         // pfnMessageEnd()
    nullptr,         // pfnWriteByte()
    nullptr,         // pfnWriteChar()
    nullptr,         // pfnWriteShort()
    nullptr,         // pfnWriteLong()
    nullptr,         // pfnWriteAngle()
    nullptr,         // pfnWriteCoord()
    nullptr,         // pfnWriteString()
    nullptr,         // pfnWriteEntity()
    nullptr,         // pfnCVarRegister()
    nullptr,         // pfnCVarGetFloat()
    nullptr,         // pfnCVarGetString()
    nullptr,         // pfnCVarSetFloat()
    nullptr,         // pfnCVarSetString()
    nullptr,         // pfnAlertMessage()
    nullptr,         // pfnEngineFprintf()
    nullptr,         // pfnPvAllocEntPrivateData()
    nullptr,         // pfnPvEntPrivateData()
    nullptr,         // pfnFreeEntPrivateData()
    nullptr,         // pfnSzFromIndex()
    nullptr,         // pfnAllocString()
    nullptr,         // pfnGetVarsOfEnt()
    nullptr,         // pfnPEntityOfEntOffset()
    nullptr,         // pfnEntOffsetOfPEntity()
    nullptr,         // pfnIndexOfEdict()
    nullptr,         // pfnPEntityOfEntIndex()
    nullptr,         // pfnFindEntityByVars()
    nullptr,         // pfnGetModelPtr()
    RegUserMsg_Post, // pfnRegUserMsg()
    nullptr,         // pfnAnimationAutomove()
    nullptr,         // pfnGetBonePosition()
    nullptr,         // pfnFunctionFromName()
    nullptr,         // pfnNameForFunction()
    nullptr,         // pfnClientPrintf()
    nullptr,         // pfnServerPrint()
    nullptr,         // pfnCmd_Args()
    nullptr,         // pfnCmd_Argv()
    nullptr,         // pfnCmd_Argc()
    nullptr,         // pfnGetAttachment()
    nullptr,         // pfnCRC32_Init()
    nullptr,         // pfnCRC32_ProcessBuffer()
    nullptr,         // pfnCRC32_ProcessByte()
    nullptr,         // pfnCRC32_Final()
    nullptr,         // pfnRandomLong()
    nullptr,         // pfnRandomFloat()
    nullptr,         // pfnSetView()
    nullptr,         // pfnTime()
    nullptr,         // pfnCrosshairAngle()
    nullptr,         // pfnLoadFileForMe()
    nullptr,         // pfnFreeFile()
    nullptr,         // pfnEndSection()
    nullptr,         // pfnCompareFileTime()
    nullptr,         // pfnGetGameDir()
    nullptr,         // pfnCvar_RegisterVariable()
    nullptr,         // pfnFadeClientVolume()
    nullptr,         // pfnSetClientMaxspeed()
    nullptr,         // pfnCreateFakeClient()
    nullptr,         // pfnRunPlayerMove()
    nullptr,         // pfnNumberOfEntities()
    nullptr,         // pfnGetInfoKeyBuffer()
    nullptr,         // pfnInfoKeyValue()
    nullptr,         // pfnSetKeyValue()
    nullptr,         // pfnSetClientKeyValue()
    nullptr,         // pfnIsMapValid()
    nullptr,         // pfnStaticDecal()
    nullptr,         // pfnPrecacheGeneric()
    nullptr,         // pfnGetPlayerUserId()
    nullptr,         // pfnBuildSoundMsg()
    nullptr,         // pfnIsDedicatedServer()
    nullptr,         // pfnCVarGetPointer()
    nullptr,         // pfnGetPlayerWONId()
    nullptr,         // pfnInfo_RemoveKey()
    nullptr,         // pfnGetPhysicsKeyValue()
    nullptr,         // pfnSetPhysicsKeyValue()
    nullptr,         // pfnGetPhysicsInfoString()
    nullptr,         // pfnPrecacheEvent()
    nullptr,         // pfnPlaybackEvent()
    nullptr,         // pfnSetFatPVS()
    nullptr,         // pfnSetFatPAS()
    nullptr,         // pfnCheckVisibility()
    nullptr,         // pfnDeltaSetField()
    nullptr,         // pfnDeltaUnsetField()
    nullptr,         // pfnDeltaAddEncoder()
    nullptr,         // pfnGetCurrentPlayer()
    nullptr,         // pfnCanSkipPlayer()
    nullptr,         // pfnDeltaFindField()
    nullptr,         // pfnDeltaSetFieldByIndex()
    nullptr,         // pfnDeltaUnsetFieldByIndex()
    nullptr,         // pfnSetGroupMask()
    nullptr,         // pfnCreateInstancedBaseline()
    nullptr,         // pfnCvar_DirectSet()
    nullptr,         // pfnForceUnmodified()
    nullptr,         // pfnGetPlayerStats()
    nullptr,         // pfnAddServerCommand()

    // Added in SDK 2.2:
    nullptr, // pfnVoice_GetClientListening()
    nullptr, // pfnVoice_SetClientListening()

    // Added for HL 1109 (no SDK update):
    nullptr, // pfnGetPlayerAuthId()

    // Added 2003/11/10 (no SDK update):
    nullptr, // pfnSequenceGet()
    nullptr, // pfnSequencePickSentence()
    nullptr, // pfnGetFileSize()
    nullptr, // pfnGetApproxWavePlayLen()
    nullptr, // pfnIsCareerMatch()
    nullptr, // pfnGetLocalizedStringLength()
    nullptr, // pfnRegisterTutorMessageShown()
    nullptr, // pfnGetTimesTutorMessageShown()
    nullptr, // pfnProcessTutorMessageDecayBuffer()
    nullptr, // pfnConstructTutorMessageDecayBuffer()
    nullptr, // pfnResetTutorMessageDecayData()

    // Added Added 2005-08-11 (no SDK update)
    nullptr, // pfnQueryClientCvarValue()
    // Added Added 2005-11-22 (no SDK update)
    nullptr, // pfnQueryClientCvarValue2()
    // Added 2009-06-17 (no SDK update)
    nullptr, // pfnEngCheckParm()
};

C_DLLEXPORT int GetEngineFunctions(enginefuncs_t *pengfuncsFromEngine, int *interfaceVersion)
{
    if (!pengfuncsFromEngine)
        return 0;

    if (*interfaceVersion != ENGINE_INTERFACE_VERSION)
    {
        *interfaceVersion = ENGINE_INTERFACE_VERSION;
        return 0;
    }

    memcpy(pengfuncsFromEngine, &gEngineFunctionsTable, sizeof(enginefuncs_t));
    return 1;
}

C_DLLEXPORT int GetEngineFunctions_Post(enginefuncs_t *pengfuncsFromEngine, int *interfaceVersion)
{
    if (!pengfuncsFromEngine)
        return 0;

    if (*interfaceVersion != ENGINE_INTERFACE_VERSION)
    {
        *interfaceVersion = ENGINE_INTERFACE_VERSION;
        return 0;
    }

    memcpy(pengfuncsFromEngine, &gEngineFunctionsTablePost, sizeof(enginefuncs_t));
    return 1;
}
