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

#include <IPluginMngr.hpp>

namespace PyMod
{
    constexpr auto PYMOD_API_VERSION = 1UL;

    class IPyGlobal
    {
    public:
        virtual ~IPyGlobal() {};
        virtual const char *getHome() const = 0;
        virtual IPluginMngr *getPluginManager() const = 0;
    };

    using getPyGlobalObject = IPyGlobal *(*)(unsigned long apiversion);
}
