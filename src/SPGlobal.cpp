/*  SPMod - SourcePawn Scripting Engine for Half-Life
 *  Copyright (C) 2018  SPMod Development Team
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

#include <spmod.hpp>

std::unique_ptr<SPGlobal> gSPGlobal;

SPGlobal::~SPGlobal()
{
#ifdef SP_POSIX
    dlclose(m_SPLibraryHandle);
#else
    //TODO: windows
#endif
}

IPluginMngr *SPGlobal::getPluginManager() const
{
    return m_pluginManager.get();
}

bool SPGlobal::addModule(sp_nativeinfo_t *natives, const char *name, sp_api_t api)
{
    //TODO: Error reporting?
    if (api > SPMOD_API_VERSION)
        return false;

    if (m_modulesNames.find(name) != m_modulesNames.end())
        return false;

    m_modulesNames.insert({ name, natives });

    return true;
}

void SPGlobal::initPluginManager()
{
    if (m_pluginManager)
        return;

    m_pluginManager = std::make_unique<PluginMngr>(m_pyScriptsDir);
}

void SPGlobal::setScriptsDir(const std::string &folder)
{
    fs::path pathToScripts(m_pyModDir);
    pathToScripts /= folder;
    m_pyScriptsDir = std::move(pathToScripts);
}

#ifdef SP_POSIX
void SPGlobal::setSPFactory(void *library, SourcePawn::ISourcePawnFactory *factory)
#else
// TODO: windows
#endif
{
    if (m_spFactory)
        return;

    m_spFactory = factory;
    m_spFactory->NewEnvironment();
    m_SPLibraryHandle = library;
};
