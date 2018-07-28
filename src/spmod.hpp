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

#pragma once

#include <IHelpers.hpp>

// Metamod & HLSDK
#include <extdll.h>
#include <meta_api.h>
// Supress warning about redefining
#undef MAX_PATH

// ReHLDS
#ifdef SP_CLANG
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#elif defined SP_GCC
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
    #pragma GCC diagnostic ignored "-Wpedantic"
#endif

#include <osconfig.h>
#include <rehlds_api.h>

#ifdef SP_CLANG
    #pragma clang diagnostic pop
#elif defined SP_GCC
    #pragma GCC diagnostic pop
#endif

#undef max
#undef min

// SPMod interface
#include <ISPGlobal.hpp>

// STL C++
#include <memory>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <exception>
#include <array>
#include <variant>
#include <string_view>
#include <fstream>
#include <regex>

#if __has_include(<filesystem>)
    #include <filesystem>
    // As of GCC 8.1 filesystem is no longer part of experimental
    #if defined SP_GCC && __GNUC__ >= 8
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

// C
#define __STDC_WANT_LIB_EXT1__
#include <cstring>
#include <cstdarg>

extern IRehldsApi *gRehldsApi;
extern const RehldsFuncs_t *gRehldsFuncs;
extern IRehldsHookchains *gRehldsHookchains;
extern IRehldsServerStatic *gRehldsServerStatic;
extern IRehldsServerData *gRehldsServerData;

extern enginefuncs_t *gpEngineFuncs;

bool initRehldsApi();
void installRehldsHooks();
void uninstallRehldsHooks();

// For convenience
using namespace SPMod;

// SPMod specific
#include <SPConfig.hpp>
#include "UtilsSystem.hpp"
#include "SPModModuleDef.hpp"
#include "LoggingSystem.hpp"
#include "PluginSystem.hpp"
#include "ForwardSystem.hpp"
#include "NativeSystem.hpp"
#include "CvarSystem.hpp"
#include "CmdSystem.hpp"
#include "TimerSystem.hpp"
#include "MenuSystem.hpp"
#include "SPGlobal.hpp"

constexpr auto gSPModAuthor = "SPMod Development Team";

#define APP_COMMIT_URL "https://github.com/Amaroq7/SPMod/commits/"

extern sp_nativeinfo_t gCoreNatives[];
extern sp_nativeinfo_t gCvarsNatives[];
extern sp_nativeinfo_t gForwardsNatives[];
extern sp_nativeinfo_t gStringNatives[];
extern sp_nativeinfo_t gMessageNatives[];
extern sp_nativeinfo_t gCmdsNatives[];
extern sp_nativeinfo_t gTimerNatives[];
extern sp_nativeinfo_t gMenuNatives[];
extern sp_nativeinfo_t gFloatNatives[];

extern int gmsgShowMenu;
extern int gmsgVGUIMenu;

// Server command function (SrvCommand.cpp)
void SPModInfoCommand();
void PluginSrvCmd();
