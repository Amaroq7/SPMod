/*  PyMod - Python Scripting Engine for Half-Life
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

#include <IPluginMngr.hpp>

#ifndef DLLEXPORT
    #ifdef _WIN32
        #define DLLEXPORT	__declspec(dllexport)
    #else
        #define DLLEXPORT	__attribute__((visibility("default")))
    #endif
#endif

#ifndef C_DLLEXPORT
    #define C_DLLEXPORT extern "C" DLLEXPORT
#endif

namespace PyMod
{
    using api_t = unsigned long;
    constexpr api_t PYMOD_API_VERSION = 1;

    using intFunction = PyObject *(*)(void);

    class IPyGlobal
    {
    public:
        virtual const char *getHome() const = 0;
        virtual const char *getModName() const = 0;
        virtual IPluginMngr *getPluginManager() const = 0;
        virtual bool addInterface(intFunction func, const char *name, api_t api = PYMOD_API_VERSION) = 0;

    protected:
        virtual ~IPyGlobal() {};
    };

    using fnPyModQuery = int (*)(IPyGlobal *pymodInstance, api_t apiversion);
    C_DLLEXPORT int PyMod_Query(IPyGlobal *pymodInstance, api_t apiversion);
}
