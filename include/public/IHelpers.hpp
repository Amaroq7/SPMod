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

#include <type_traits>

// Platform defines
#if defined _WIN32
    #define SP_WINDOWS
#elif defined __linux__
    #define SP_LINUX
    #define SP_POSIX
#endif

// Compiler defines
#if defined SP_POSIX
    #if defined __clang__
        #define SP_CLANG
    #else
        #define SP_GCC
    #endif
#elif defined SP_WINDOWS
    #ifdef _MSC_VER
        #define SP_MSVC
    #endif
#endif

#if !defined SPMOD_API
    #if defined SP_POSIX
        #define SPMOD_API __attribute__((visibility("default")))
    #else
        #define SPMOD_API __declspec(dllexport)
    #endif
#endif
