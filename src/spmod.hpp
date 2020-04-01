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

#pragma once

#include <IHelpers.hpp>

// Metamod & HLSDK
#if defined SP_MSVC
    #pragma warning(push)
    #pragma warning(disable:4100)
#endif
#include <osconfig.h>
#if defined SP_MSVC
    #pragma warning(pop)
#endif

#include <extdll.h>
#include <meta_api.h>
// Supress warning about redefining
#undef MAX_PATH

// ReHLDS
#include <usercmd.h>
#include <rehlds_api.h>

#undef max
#undef min

// SPMod interface
#include <ISPGlobal.hpp>

// C
#define __STDC_WANT_LIB_EXT1__
#include <cstring>
#include <cstdarg>

// STL C++
#include <memory>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <exception>
#include <fstream>
#include <stack>

#include <yaml-cpp/yaml.h>

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

#if defined SP_MSVC
    #pragma warning(push)
    #pragma warning(disable:4250)
#endif

// engine implementations
#include "engine/EntVars.hpp"
#include "engine/Edict.hpp"
#include "engine/TraceResult.hpp"
#include "engine/Funcs.hpp"
#include "engine/Globals.hpp"
#include "engine/Engine.hpp"

// valve entities
#include "valve/BaseEntity.hpp"
#include "valve/BaseDelay.hpp"
#include "valve/BaseAnimating.hpp"
#include "valve/BaseToggle.hpp"
#include "valve/BaseMonster.hpp"
#include "valve/BasePlayer.hpp"

// metamod
#include "metamod/Funcs.hpp"
#include "metamod/Metamod.hpp"

#if defined SP_MSVC
    #pragma warning(pop)
#endif


// SPMod
#include "UtilsSystem.hpp"
#include "LoggingSystem.hpp"
#include "ForwardSystem.hpp"
#include "CvarSystem.hpp"
#include "CmdSystem.hpp"
#include "TimerSystem.hpp"
#include "MenuSystem.hpp"
#include "PlayerSystem.hpp"
#include "MessageSystem.hpp"
#include "ExtensionSystem.hpp"
#include "NativeProxy.hpp"
#include "VTableHookManager.hpp"
#include "SPGlobal.hpp"

constexpr const char *gSPModAuthor = "SPMod Development Team";
constexpr const char *APP_COMMIT_URL = "https://github.com/Amaroq7/SPMod/commits/";
constexpr const char *gSPModLoggerName = "SPMod";

extern int gmsgShowMenu;
extern int gmsgVGUIMenu;
