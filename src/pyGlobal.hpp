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

#include <pymod.hpp>

class PluginMngr;

class PyGlobal : public PyMod::IPyGlobal
{
public:
    PyGlobal() = delete;
    PyGlobal(const fs::path &dllDir);
    ~PyGlobal() = default;

    //IPyGlobal
    const char *getHome() const override;
    const char *getModName() const override;
    PyMod::IPluginMngr *getPluginManager() const override;

    void initializePluginManager();
    void setModName(const std::string &name);
private:
    fs::path m_pyModDir;
    std::unique_ptr<PluginMngr> m_pluginManager;
    std::string m_modName;
};

extern std::unique_ptr<PyGlobal> gPyGlobal;
