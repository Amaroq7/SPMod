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

namespace PyMod
{
    class IPluginMngr
    {
    public:
        virtual ~IPluginMngr() {};
        //Plugin *loadPlugin(const char *name, char *error, size_t size);
        //void unloadPlugin(size_t index);

        //Plugin *findPlugin(size_t index);
        //Plugin *findPlugin(const char *name);

        virtual size_t getPluginsNum() const = 0;
        //const auto& getPlugin(size_t index) const { return m_plugins; }
    };
}
