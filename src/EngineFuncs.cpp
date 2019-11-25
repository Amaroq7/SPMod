/*
 *  Copyright (C) 2018 SPMod Development Team
 *
 *  This file is part of SPMod.
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

const char *EngineFuncs::getArg(int arg) const
{
    return CMD_ARGV(arg);
}

const char *EngineFuncs::getArgs() const
{
    return CMD_ARGS();
}

int EngineFuncs::getArgc() const
{
    return CMD_ARGC();
}

void EngineFuncs::serverPrint(const char *msg) const
{
    SERVER_PRINT(msg);
}

int EngineFuncs::precacheModel(const char *model) const
{
    return PRECACHE_MODEL(model);
}

int EngineFuncs::precacheSound(const char *sound) const
{
    return PRECACHE_SOUND(sound);
}

int EngineFuncs::precacheGeneric(const char *generic) const
{
    return PRECACHE_GENERIC(generic);
}

void EngineFuncs::changeLevel(const char *level) const
{
    CHANGE_LEVEL(level, nullptr);
}

void EngineFuncs::serverCommand(const char* cmd) const
{
    SERVER_COMMAND(cmd);
}

void EngineFuncs::serverExecute() const
{
    SERVER_EXECUTE();
}

void EngineFuncs::registerSrvCommand(const char *cmd) const
{
    REG_SVR_COMMAND(cmd, PluginSrvCmd);
}

void EngineFuncs::messageBegin(int msgDest, int msgType, const float *pOrigin, IEdict *pEdict) const
{
    MESSAGE_BEGIN(msgDest, msgType, pOrigin, INDEXENT(pEdict->getIndex()));
}

void EngineFuncs::messageEnd() const
{
    MESSAGE_END();
}

void EngineFuncs::writeByte(int byteArg) const
{
    WRITE_BYTE(byteArg);
}

void EngineFuncs::writeChar(int charArg) const
{
    WRITE_CHAR(charArg);
}

void EngineFuncs::writeShort(int shortArg) const
{
    WRITE_SHORT(shortArg);
}

void EngineFuncs::writeLong(int longArg) const
{
    WRITE_LONG(longArg);
}

void EngineFuncs::writeEntity(int entArg) const
{
    WRITE_ENTITY(entArg);
}

void EngineFuncs::writeAngle(float angleArg) const
{
    WRITE_ANGLE(angleArg);
}

void EngineFuncs::writeCoord(float coordArg) const
{
    WRITE_COORD(coordArg);
}

void EngineFuncs::writeString(const char *strArg) const
{
    WRITE_STRING(strArg);
}

const char *EngineFuncsHooked::getArg(int arg) const
{
    return gpEngineFuncs->pfnCmd_Argv(arg);
}

const char *EngineFuncsHooked::getArgs() const
{
    return gpEngineFuncs->pfnCmd_Args();
}

int EngineFuncsHooked::getArgc() const
{
    return gpEngineFuncs->pfnCmd_Argc();
}

void EngineFuncsHooked::serverPrint(const char *msg) const
{
    gpEngineFuncs->pfnServerPrint(msg);
}

int EngineFuncsHooked::precacheModel(const char *model) const
{
    return gpEngineFuncs->pfnPrecacheModel(model);
}

int EngineFuncsHooked::precacheSound(const char *sound) const
{
    return gpEngineFuncs->pfnPrecacheSound(sound);
}

int EngineFuncsHooked::precacheGeneric(const char *generic) const
{
    return gpEngineFuncs->pfnPrecacheGeneric(generic);
}

void EngineFuncsHooked::changeLevel(const char *level) const
{
    gpEngineFuncs->pfnChangeLevel(level, nullptr);
}

void EngineFuncsHooked::serverCommand(const char* cmd) const
{
    gpEngineFuncs->pfnServerCommand(cmd);
}

void EngineFuncsHooked::serverExecute() const
{
    gpEngineFuncs->pfnServerExecute();
}

void EngineFuncsHooked::registerSrvCommand(const char *cmd) const
{
    gpEngineFuncs->pfnAddServerCommand(cmd, PluginSrvCmd);
}

void EngineFuncsHooked::messageBegin(int msgDest, int msgType, const float *pOrigin, IEdict *pEdict) const
{
    gpEngineFuncs->pfnMessageBegin(msgDest, msgType, pOrigin, INDEXENT(pEdict->getIndex()));
}

void EngineFuncsHooked::messageEnd() const
{
    gpEngineFuncs->pfnMessageEnd();
}

void EngineFuncsHooked::writeByte(int byteArg) const
{
    gpEngineFuncs->pfnWriteByte(byteArg);
}

void EngineFuncsHooked::writeChar(int charArg) const
{
    gpEngineFuncs->pfnWriteChar(charArg);
}

void EngineFuncsHooked::writeShort(int shortArg) const
{
    gpEngineFuncs->pfnWriteShort(shortArg);
}

void EngineFuncsHooked::writeLong(int longArg) const
{
    gpEngineFuncs->pfnWriteLong(longArg);
}

void EngineFuncsHooked::writeEntity(int entArg) const
{
    gpEngineFuncs->pfnWriteEntity(entArg);
}

void EngineFuncsHooked::writeAngle(float angleArg) const
{
    gpEngineFuncs->pfnWriteAngle(angleArg);
}

void EngineFuncsHooked::writeCoord(float coordArg) const
{
    gpEngineFuncs->pfnWriteCoord(coordArg);
}

void EngineFuncsHooked::writeString(const char *strArg) const
{
    gpEngineFuncs->pfnWriteString(strArg);
}

float EngineGlobals::getTime() const
{
    return gpGlobals->time;
}

const char *EngineGlobals::getMapName() const
{
    return STRING(gpGlobals->mapname);
}