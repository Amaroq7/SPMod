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

#pragma once

#include <unordered_map>

#include <ISPGlobal.hpp>

#if __has_include(<filesystem>)
    #include <filesystem>
    // As of GCC 8.1 and Clang 7 filesystem is no longer part of experimental
    #if (defined SP_GCC && __GNUC__ >= 8) || (defined SP_CLANG && __clang_major__ >= 7)
namespace fs = std::filesystem;
    #else // Some compilers still have filesystem within experimental namespace like MSVC
namespace fs = std::experimental::filesystem;
    #endif
#elif __has_include(<experimental/filesystem>)
    #include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
    #error Filesystem header missing
#endif

#include <lua/lua.hpp>
#include "PluginMngr.hpp"
#include "AdapterInterface.hpp"

extern SPMod::ISPGlobal *gSPGlobal;
extern SPMod::ILogger *gLogger;
extern SPLuaAdapter::Interface gInterface;
extern lua_State *gLuaState;

struct LuaAdapterCFucntion
{
    const char *name;
    lua_CFunction func;
};
extern LuaAdapterCFucntion gCoreNatives[];