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

#include <pymod.hpp>

std::unique_ptr<PyGlobal> gPyGlobal;

void PyGlobal::initializePluginManager()
{
    if (m_pluginManager)
        return;

    m_pluginManager = std::make_unique<PluginMngr>(m_pyModDir / "scripts");
}

IPluginMngr *PyGlobal::getPluginManager() const
{
    return m_pluginManager.get();
}

bool PyGlobal::addModule(intFunction func, const char *name, api_t api)
{
    //TODO: Error reporting?
    if (api != PYMOD_API_VERSION)
        return false;

    if (m_modulesNames.find(name) != m_modulesNames.end())
        return false;

    m_modulesNames.insert({ name, func });

    return true;
}

void PyGlobal::setScriptsDir(const std::string &folder)
{
    fs::path pathToScripts(m_pyModDir);
    pathToScripts /= folder;
    m_pyScriptsDir = std::move(pathToScripts);
}
