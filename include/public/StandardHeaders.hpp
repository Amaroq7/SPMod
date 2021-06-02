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

#include <cinttypes>
#include <cstddef>
#include <functional>
#include <any>
#include <string>
#include <variant>
#include <array>
#include <type_traits>
#include <regex>
#include <charconv>
#include <optional>
#include <unordered_map>
#include <exception>
#include <memory>
#include <utility>
#include <fstream>

#include <IHelpers.hpp>

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