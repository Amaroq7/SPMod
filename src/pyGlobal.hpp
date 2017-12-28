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

#include <pymod.hpp>

class PluginMngr;

class PyGlobal : public IPyGlobal
{
public:
    PyGlobal() = delete;
    PyGlobal(const fs::path &dllDir) : m_pyModDir(dllDir.parent_path().parent_path()) { }
    ~PyGlobal() = default;

    // IPyGlobal
    const char *getHome() const override { return m_pyModDir.c_str(); }
    const char *getModName() const override { return m_modName.c_str(); }
    IPluginMngr *getPluginManager() const override;
    bool addModule(intFunction func, const char *name, api_t api) override;

    // PyGlobal
    void initializePluginManager();
    void setModName(const std::string &name) { m_modName = name; }
    void setScriptsDir(const std::string &folder);
    const auto &getModulesList() const { return m_modulesNames; }
    
private:
    fs::path m_pyScriptsDir;
    fs::path m_pyModDir;
    std::unique_ptr<PluginMngr> m_pluginManager;
    std::string m_modName;
    std::unordered_map<std::string, intFunction> m_modulesNames;
};

extern std::unique_ptr<PyGlobal> gPyGlobal;
