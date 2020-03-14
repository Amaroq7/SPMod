/*
 *  Copyright (C) 2018-2019 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SPMod is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with SPMod.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <ISPGlobal.hpp>

#if __has_include(<filesystem>)
    #include <filesystem>
    // As of GCC 8.1, Clang 7 and MSVC 2019 filesystem is no longer part of experimental
    #if (defined SP_GCC && __GNUC__ >= 8) || (defined SP_CLANG && __clang_major__ >= 7) ||                             \
        (defined SP_MSVC && _MSC_VER >= 1920)
namespace fs = std::filesystem;
    #else // Some compilers still have filesystem within experimental namespace like MSVC 2017
namespace fs = std::experimental::filesystem;
    #endif
#elif __has_include(<experimental/filesystem>)
    #include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
    #error Filesystem header missing
#endif

#include <unordered_map>
#include <array>
#include <vector>

#if defined SP_POSIX
    #include <dlfcn.h>
#endif

#include "SourcePawnAPI.hpp"
#include "DebugListener.hpp"
#include "PluginSystem.hpp"
#include "ModuleInterface.hpp"
#include "TypeHandler.hpp"
#include "PrintfImpl.hpp"
#include "Listeners.hpp"

extern std::unique_ptr<SPExt::ModuleInterface> gModuleInterface;
extern SPMod::ISPGlobal *gSPGlobal;
extern std::unique_ptr<SourcePawnAPI> gSPAPI;

// ForwardNatives.cpp
extern TypeHandler<SPMod::IForward> gForwardHandlers;

// CmdNatives.cpp
extern TypeHandler<SPMod::ICommand> gCommandHandlers;

// CvarNatives.cpp
extern TypeHandler<SPMod::ICvar> gCvarsHandlers;
extern std::unordered_multimap<SPMod::ICvar *, SourcePawn::IPluginFunction *> gCvarPluginsCallbacks;

// MenuNatives.cpp
extern TypeHandler<SPMod::IMenu> gMenuHandlers;

// TimerNatives.cpp
extern TypeHandler<SPMod::ITimer> gTimerHandlers;

extern SPMod::ILogger *gSPLogger;

constexpr std::size_t menuPackItem(std::size_t menuid, std::size_t itemid)
{
    return (menuid << 16 | itemid);
}

constexpr void menuUnpackItem(std::size_t index, cell_t &menuid, cell_t &itemid)
{
    menuid = index >> 16;
    itemid = index & 0xFFFF;
}

// Natives
extern sp_nativeinfo_t gCmdsNatives[];
extern sp_nativeinfo_t gCoreNatives[];
extern sp_nativeinfo_t gCvarsNatives[];
extern sp_nativeinfo_t gFloatNatives[];
extern sp_nativeinfo_t gForwardsNatives[];
extern sp_nativeinfo_t gMenuNatives[];
extern sp_nativeinfo_t gMessageNatives[];
extern sp_nativeinfo_t gPlayerNatives[];
extern sp_nativeinfo_t gStringNatives[];
extern sp_nativeinfo_t gTimerNatives[];