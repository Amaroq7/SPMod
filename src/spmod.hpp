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
#include <extdll.h>
#include <meta_api.h>
// Supress warning about redefining
#undef MAX_PATH

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
#include <functional>

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
#include "MessageSystem.hpp"
#include "ExtensionSystem.hpp"
#include "NativeProxy.hpp"
#include "EngineFuncs.hpp"
#include "MetaFuncs.hpp"
#include "SPGlobal.hpp"

constexpr const char *gSPModAuthor = "SPMod Development Team";
constexpr const char *APP_COMMIT_URL = "https://github.com/Amaroq7/SPMod/commits/";
constexpr const char *gSPModLoggerName = "SPMod";

extern int gmsgShowMenu;
extern int gmsgVGUIMenu;
