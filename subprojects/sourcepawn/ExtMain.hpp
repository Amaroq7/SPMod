/*
 *  Copyright (C) 2018-2020 SPMod Development Team
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
#include <unordered_map>
#include <array>
#include <vector>
#include <cstdarg>
#include <cmath>

#if defined SP_POSIX
    #include <dlfcn.h>
#endif

#include "SourcePawnAPI.hpp"
#include "DebugListener.hpp"
#include "PluginSystem.hpp"
#include "AdapterInterface.hpp"
#include "TypeHandler.hpp"
#include "PrintfImpl.hpp"
#include "Listeners.hpp"

extern std::unique_ptr<SPExt::AdapterInterface> gAdapterInterface;
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

constexpr const char *gSPExtLoggerName = "SPExt";

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