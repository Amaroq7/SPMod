/*
 *  Copyright (C) 2019 SPMod Development Team
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

#include "ExtMain.hpp"

SPMod::ISPGlobal *gSPGlobal;
SPMod::ILogger *gLogger;
SPLuaAdapter::Interface gInterface;
lua_State *gLuaState;

static void *luaAllocFunc(void *ud [[maybe_unused]], void *ptr, size_t osize [[maybe_unused]], size_t nsize)
{
    if (!nsize)
    {
        free(ptr);
        return nullptr;
    }
    else
    {
        return realloc(ptr, nsize);
    }
}

SPMOD_API SPMod::ExtQueryValue SPMod_Query(SPMod::ISPGlobal *spmodInstance)
{
    gSPGlobal = spmodInstance;

    if (!gSPGlobal->getLoggerManager()->isVersionCompatible(SPMod::ILoggerMngr::VERSION))
    {
        return SPMod::ExtQueryValue::DontLoad;
    }

    gSPGlobal->registerAdapter(&gInterface);
    return SPMod::ExtQueryValue::SPModAdapter;
}

SPMOD_API bool SPMod_Init()
{
    gLogger = gSPGlobal->getLoggerManager()->getLogger("SPLua");
#if defined NDEBUG
    gLogger->setLogLevel(SPMod::LogLevel::Info);
#else
    gLogger->setLogLevel(SPMod::LogLevel::Debug);
#endif

    gLuaState = lua_newstate(luaAllocFunc, nullptr);
    if (!gLuaState)
    {
        gLogger->logToBoth(SPMod::LogLevel::Error, "Cannot initialize Lua");
        return false;
    }

    luaL_checkversion(gLuaState);

    gLogger->logToConsole(SPMod::LogLevel::Info, "%s", LUA_VERSION);

    luaL_openlibs(gLuaState);

    std::size_t i = 0;
    do
    {
        lua_register(gLuaState, gCoreNatives[i].name, gCoreNatives[i].func);
    } while (gCoreNatives[++i].name);

    return true;
}

SPMOD_API void SPMod_End()
{
    lua_close(gLuaState);
}