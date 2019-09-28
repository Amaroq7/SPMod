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

#pragma once

// Platform defines
#ifdef _WIN32
    #define SP_WINDOWS
#elif defined __linux__
    #define SP_LINUX
    #define SP_POSIX
#endif

// Compiler defines
#ifdef SP_POSIX
    #ifdef __clang__
        #define SP_CLANG
    #elif defined __GNUC__
        #define SP_GCC
    #endif
#elif defined SP_WINDOWS
    #ifdef _MSC_VER
        #define SP_MSVC
    #endif
#endif

#ifndef SPMOD_API
    #ifdef SP_POSIX
        #define SPMOD_API	extern "C" __attribute__((visibility("default")))
    #else
        #define SPMOD_API	extern "C" __declspec(dllexport)
    #endif
#endif
