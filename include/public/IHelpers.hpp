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
    #elif defined __GNUC__
        #define SP_GCC
    #endif
#elif defined SP_WINDOWS
    #ifdef _MSC_VER
        #define SP_MSVC
    #endif
#endif

#if !defined SPMOD_API
    #if defined SP_POSIX
        #define SPMOD_API extern "C" __attribute__((visibility("default")))
    #else
        #define SPMOD_API extern "C" __declspec(dllexport)
    #endif
#endif

// Console colors
#if defined SP_POSIX
constexpr const char *CNSL_DEFAULT = "\033[39m";
constexpr const char *CNSL_BLACK = "\033[30m";
constexpr const char *CNSL_RED = "\033[31m";
constexpr const char *CNSL_GREEN = "\033[32m";
constexpr const char *CNSL_YELLOW = "\033[33m";
constexpr const char *CNSL_BLUE = "\033[34m";
constexpr const char *CNSL_MAGENTA = "\033[35m";
constexpr const char *CNSL_CYAN = "\033[36m";
constexpr const char *CNSL_LGRAY = "\033[37m";
constexpr const char *CNSL_DGRAY = "\033[90m";
constexpr const char *CNSL_LRED = "\033[91m";
constexpr const char *CNSL_LGREEN = "\033[92m";
constexpr const char *CNSL_LYELLOW = "\033[93m";
constexpr const char *CNSL_LBLUE = "\033[94m";
constexpr const char *CNSL_LMAGENTA = "\033[95m";
constexpr const char *CNSL_LCYAN = "\033[96m";
constexpr const char *CNSL_WHITE = "\033[97m";
constexpr const char *CNSL_BOLD = "\033[1m";
constexpr const char *CNSL_RESET = "\033[0m";
#else
constexpr const char *CNSL_DEFAULT = "";
constexpr const char *CNSL_BLACK = "";
constexpr const char *CNSL_RED = "";
constexpr const char *CNSL_GREEN = "";
constexpr const char *CNSL_YELLOW = "";
constexpr const char *CNSL_BLUE = "";
constexpr const char *CNSL_MAGENTA = "";
constexpr const char *CNSL_CYAN = "";
constexpr const char *CNSL_LGRAY = "";
constexpr const char *CNSL_DGRAY = "";
constexpr const char *CNSL_LRED = "";
constexpr const char *CNSL_LGREEN = "";
constexpr const char *CNSL_LYELLOW = "";
constexpr const char *CNSL_LBLUE = "";
constexpr const char *CNSL_LMAGENTA = "";
constexpr const char *CNSL_LCYAN = "";
constexpr const char *CNSL_WHITE = "";
constexpr const char *CNSL_BOLD = "";
constexpr const char *CNSL_RESET = "";
#endif

template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
constexpr bool operator&(const T lhs, const T rhs)
{
    using enumType = std::underlying_type_t<T>;
    return static_cast<bool>(static_cast<enumType>(lhs) & static_cast<enumType>(rhs));
}

template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
constexpr T operator|(const T lhs, const T rhs)
{
    return static_cast<T>(static_cast<std::underlying_type_t<T>>(lhs) | static_cast<std::underlying_type_t<T>>(rhs));
}

template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
constexpr T operator^(const T lhs, const T rhs)
{
    return static_cast<T>(static_cast<std::underlying_type_t<T>>(lhs) ^ static_cast<std::underlying_type_t<T>>(rhs));
}

template<typename T, typename S, typename = std::enable_if_t<std::is_enum_v<T>>>
constexpr bool operator==(const S lhs, const T rhs)
{
    return lhs == static_cast<std::underlying_type_t<T>>(rhs);
}

template<typename T, typename S, typename = std::enable_if_t<std::is_enum_v<T>>>
constexpr bool operator!=(const S lhs, const T rhs)
{
    return lhs != static_cast<std::underlying_type_t<T>>(rhs);
}
