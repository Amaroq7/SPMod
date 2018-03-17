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

// Metamod & HLSDK
#include <extdll.h>
#include <meta_api.h>

// ReHLDS
#include <osconfig.h>
#include <rehlds_api.h>

#undef max
#undef min

// STL C++
#include <memory>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <experimental/filesystem>
#include <exception>
#include <array>
#include <variant>
#include <string_view>
#include <fstream>
#include <any>
#include <optional>

namespace fs = std::experimental::filesystem;

// C
#include <cstring>
#include <cstdarg>

// Sourcepawn
#include <sp_vm_api.h>

extern IRehldsApi *gRehldsApi;
extern const RehldsFuncs_t *gRehldsFuncs;
extern IRehldsHookchains *gRehldsHookchains;
extern IRehldsServerStatic *gRehldsServerStatic;
extern IRehldsServerData *gRehldsServerData;

extern bool initRehldsApi();

// SPMod interface
#include <ISPGlobal.hpp>

// Aliases for convenience
using namespace SPMod;

class Plugin;
using fwdOwnerVariant = std::variant<std::weak_ptr<Plugin>, IModuleInterface *>;
using fwdParamTypeList = std::array<IForward::ParamType, SP_MAX_EXEC_PARAMS>;
using fwdInitParamsList = std::initializer_list<IForward::ParamType>;

// SPMod specific
#include "SPModModuleDef.hpp"
#include "LoggingSystem.hpp"
#include "PluginSystem.hpp"
#include "ForwardSystem.hpp"
#include "SPGlobal.hpp"

constexpr auto gSPModAuthor = "SPMod Development Team";
constexpr auto gSPModVersion = "0.0.1";

// Core natives (coreNatives.cpp)
extern sp_nativeinfo_t gCoreNatives[];

// Server command function (SrvCommand.cpp)
void SPModInfoCommand();
