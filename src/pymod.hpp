/*  PyMod - Python scripting engine for Half-Life
 *  Copyright (C) 2018  PyMod Development Team
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

// STL C++
#include <memory>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <experimental/filesystem>
#include <exception>

namespace fs = std::experimental::filesystem;

// C
#include <cstring>

// Python
#include <Python.h>

// Metamod & HLSDK
#include <extdll.h>
#include <meta_api.h>

extern globalvars_t *gpGlobals;

// PyMod interface
#include <IPyGlobal.hpp>

using namespace PyMod;

// PyMod specific
#include <pluginMngr.hpp>
#include <pyGlobal.hpp>

constexpr auto gPyModAuthor = "PyMod Development Team";
constexpr auto gPyModVersion = "1.0.0";

// Core module (coreNatives.cpp)
PyMODINIT_FUNC PyInit_core();
