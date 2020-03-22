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

std::string_view EngineFuncs::getArg(std::uint32_t arg) const
{
    return CMD_ARGV(arg);
}

std::string_view EngineFuncs::getArgs() const
{
    return CMD_ARGS();
}

std::uint32_t EngineFuncs::getArgc() const
{
    return CMD_ARGC();
}

void EngineFuncs::serverPrint(std::string_view msg) const
{
    SERVER_PRINT(msg.data());
}

std::uint32_t EngineFuncs::precacheModel(std::string_view model) const
{
    return PRECACHE_MODEL(model.data());
}

std::uint32_t EngineFuncs::precacheSound(std::string_view sound) const
{
    return PRECACHE_SOUND(sound.data());
}

std::uint32_t EngineFuncs::precacheGeneric(std::string_view generic) const
{
    return PRECACHE_GENERIC(generic.data());
}

void EngineFuncs::changeLevel(std::string_view level) const
{
    CHANGE_LEVEL(level.data(), nullptr);
}

void EngineFuncs::serverCommand(std::string_view cmd) const
{
    SERVER_COMMAND(cmd.data());
}

void EngineFuncs::serverExecute() const
{
    SERVER_EXECUTE();
}

void EngineFuncs::registerSrvCommand(std::string_view cmd) const
{
    REG_SVR_COMMAND(cmd.data(), PluginSrvCmd);
}

void EngineFuncs::messageBegin(MessageDest msgDest, std::uint32_t msgType, const float *pOrigin, IEdict *pEdict) const
{
    MESSAGE_BEGIN(static_cast<std::int32_t>(msgDest), msgType, pOrigin, INDEXENT(pEdict->getIndex()));
}

void EngineFuncs::messageEnd() const
{
    MESSAGE_END();
}

void EngineFuncs::writeByte(std::int32_t byteArg) const
{
    WRITE_BYTE(byteArg);
}

void EngineFuncs::writeChar(std::int32_t charArg) const
{
    WRITE_CHAR(charArg);
}

void EngineFuncs::writeShort(std::int32_t shortArg) const
{
    WRITE_SHORT(shortArg);
}

void EngineFuncs::writeLong(std::int32_t longArg) const
{
    WRITE_LONG(longArg);
}

void EngineFuncs::writeEntity(std::int32_t entArg) const
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

void EngineFuncs::writeString(std::string_view strArg) const
{
    WRITE_STRING(strArg.data());
}

std::string_view EngineFuncsHooked::getArg(std::uint32_t arg) const
{
    return gpEngineFuncs->pfnCmd_Argv(arg);
}

std::string_view EngineFuncsHooked::getArgs() const
{
    return gpEngineFuncs->pfnCmd_Args();
}

std::uint32_t EngineFuncsHooked::getArgc() const
{
    return gpEngineFuncs->pfnCmd_Argc();
}

void EngineFuncsHooked::serverPrint(std::string_view msg) const
{
    gpEngineFuncs->pfnServerPrint(msg.data());
}

std::uint32_t EngineFuncsHooked::precacheModel(std::string_view model) const
{
    return gpEngineFuncs->pfnPrecacheModel(model.data());
}

std::uint32_t EngineFuncsHooked::precacheSound(std::string_view sound) const
{
    return gpEngineFuncs->pfnPrecacheSound(sound.data());
}

std::uint32_t EngineFuncsHooked::precacheGeneric(std::string_view generic) const
{
    return gpEngineFuncs->pfnPrecacheGeneric(generic.data());
}

void EngineFuncsHooked::changeLevel(std::string_view level) const
{
    gpEngineFuncs->pfnChangeLevel(level.data(), nullptr);
}

void EngineFuncsHooked::serverCommand(std::string_view cmd) const
{
    gpEngineFuncs->pfnServerCommand(cmd.data());
}

void EngineFuncsHooked::serverExecute() const
{
    gpEngineFuncs->pfnServerExecute();
}

void EngineFuncsHooked::registerSrvCommand(std::string_view cmd) const
{
    gpEngineFuncs->pfnAddServerCommand(cmd.data(), PluginSrvCmd);
}

void EngineFuncsHooked::messageBegin(MessageDest msgDest,
                                     std::uint32_t msgType,
                                     const float *pOrigin,
                                     IEdict *pEdict) const
{
    gpEngineFuncs->pfnMessageBegin(static_cast<std::int32_t>(msgDest), msgType, pOrigin, INDEXENT(pEdict->getIndex()));
}

void EngineFuncsHooked::messageEnd() const
{
    gpEngineFuncs->pfnMessageEnd();
}

void EngineFuncsHooked::writeByte(std::int32_t byteArg) const
{
    gpEngineFuncs->pfnWriteByte(byteArg);
}

void EngineFuncsHooked::writeChar(std::int32_t charArg) const
{
    gpEngineFuncs->pfnWriteChar(charArg);
}

void EngineFuncsHooked::writeShort(std::int32_t shortArg) const
{
    gpEngineFuncs->pfnWriteShort(shortArg);
}

void EngineFuncsHooked::writeLong(std::int32_t longArg) const
{
    gpEngineFuncs->pfnWriteLong(longArg);
}

void EngineFuncsHooked::writeEntity(std::int32_t entArg) const
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

void EngineFuncsHooked::writeString(std::string_view strArg) const
{
    gpEngineFuncs->pfnWriteString(strArg.data());
}

float EngineGlobals::getTime() const
{
    return gpGlobals->time;
}

std::string_view EngineGlobals::getMapName() const
{
    return STRING(gpGlobals->mapname);
}