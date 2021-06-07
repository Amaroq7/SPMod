/*
 *  Copyright (C) 2020 Metamod++ Development Team
 *
 *  This file is part of Metamod++.
 *
 *  Metamod++ is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  Metamod++ is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with Metamod++.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <cinttypes>

#if defined _WIN32
    #define DLLEXPORT   __declspec(dllexport)
#elif defined __linux__
    #define DLLEXPORT __attribute__ ((visibility ("default")))
    #define WINAPI
#endif
#define C_DLLEXPORT   extern "C" DLLEXPORT

#define META_STRONG_TYPEDEF(_base, _type)                                  \
struct _type {                                                             \
    _type() = default;                                                     \
    _type(const _base v) : value(v) {}                                     \
    _type(const _type &v) = default;                                       \
    _type &operator=(const _type &rhs) = default;                          \
    _type &operator=(const _base &rhs) { value = rhs; return *this; }      \
    operator const _base & () const { return value; }                      \
    bool operator==(const _type &rhs) const { return value == rhs.value; } \
    bool operator==(const _base &rhs) const { return value == rhs; }       \
    bool operator<(const _type &rhs) const { return value < rhs.value; }   \
    _base value;                                                           \
    using BaseType = _base;                                                \
};

#define META_STRONG_TYPEDEF_PTR(_base, _type)                              \
struct _type {                                                             \
    _type() = default;                                                     \
    _type(_base *v) : value(v) {}                                           \
    _type(const _type &v) = default;                                       \
    _type &operator=(const _type &rhs) = default;                          \
    _type &operator=(_base *rhs) { value = rhs; return *this; }             \
    operator _base *() const { return value; }                              \
    bool operator==(const _type &rhs) const { return value == rhs.value; } \
    bool operator==(const _base *rhs) const { return value == rhs; }       \
    bool operator<(const _type &rhs) const { return value < rhs.value; }   \
    _base *value;                                                           \
    using BaseType = _base *;                                                \
};

namespace Metamod
{
    enum class FuncCallType : std::uint8_t
    {
        Direct = 0,
        Hooks
    };
}
