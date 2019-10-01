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

float EngineGlobals::getTime() const
{
    return gpGlobals->time;
}