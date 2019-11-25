/*
 *  Copyright (C) 2018 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
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

#if defined SP_POSIX
    #undef _vsnprintf
#else
    #define _vsnprintf(buffer, count, format, argptr) _vsnprintf_s(buffer, count, count, format, argptr)
#endif

#if defined SP_POSIX
    #define ULONG typedefWorkaround
#endif

// ReHLDS
#include <osconfig.h>
#include <usercmd.h>
#include <rehlds_api.h>

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
#include "Edict.hpp"
#include "UtilsSystem.hpp"
#include "LoggingSystem.hpp"
#include "ForwardSystem.hpp"
#include "CvarSystem.hpp"
#include "CmdSystem.hpp"
#include "TimerSystem.hpp"
#include "MenuSystem.hpp"
#include "PlayerSystem.hpp"
#include "ExtensionSystem.hpp"
#include "NativeProxy.hpp"
#include "EngineFuncs.hpp"
#include "MetaFuncs.hpp"
#include "SPGlobal.hpp"

constexpr auto gSPModAuthor = "SPMod Development Team";

constexpr const char *APP_COMMIT_URL = "https://github.com/Amaroq7/SPMod/commits/";

extern int gmsgShowMenu;
extern int gmsgVGUIMenu;

// Server command function (SrvCommand.cpp)
void SPModInfoCommand();
void PluginSrvCmd();
