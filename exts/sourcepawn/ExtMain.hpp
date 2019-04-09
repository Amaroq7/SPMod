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

#include <unordered_map>

#include <dlfcn.h>

#include <extdll.h>
#include <ISPGlobal.hpp>

#include "SourcePawnAPI.hpp"
#include "DebugListener.hpp"
#include "ModuleInterface.hpp"
#include "TypeHandler.hpp"
#include "PrintfImpl.hpp"
#include "Listeners.hpp"

extern std::unique_ptr<ModuleInterface> gModuleInterface;
extern SPMod::ISPGlobal *gSPGlobal;

// ForwardNatives.cpp
extern TypeHandler<SPMod::IForward> gForwardHandlers;

// CvarNatives.cpp
extern TypeHandler<SPMod::ICvar> gCvarsHandlers;
extern std::unordered_multimap<SPMod::ICvar *, SourcePawn::IPluginFunction *> gCvarPluginsCallbacks;

// MenuNatives.cpp
extern TypeHandler<IMenu> gMenuHandlers;
extern std::unordered_map<SPMod::IMenu *, SourcePawn::IPluginFunction *> gMenuPluginHandlers;
extern std::unordered_map<SPMod::IMenuItem *, SourcePawn::IPluginFunction *> gMenuItemPluginHandlers;

constexpr std::size_t menuPackItem(std::size_t menuid, std::size_t itemid)
{
    return (menuid << 16 | itemid);
}

constexpr std::size_t menuUnpackItem(std::size_t index, std::size_t &menuid, std::size_t &itemid)
{
    menuid = index >> 16;
    itemid = index & 0xFFFF;
}
