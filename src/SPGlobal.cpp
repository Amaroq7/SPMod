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

#include "spmod.hpp"

std::unique_ptr<SPGlobal> gSPGlobal;

bool SPGlobal::addModule(IModuleInterface *interface)
{
    //TODO: Error reporting?
    if (interface->getInterfaceVersion() > SPMOD_API_VERSION)
        return false;

    auto *name = interface->getName();
    if (m_modulesNames.find(name) != m_modulesNames.end())
        return false;

    NativeDef nativesDef;
    nativesDef.natives = interface->getNatives();
    nativesDef.num = interface->getNativesNum();

    m_modulesNames.insert(std::make_pair(name, nativesDef));

    return true;
}

void SPGlobal::initPluginManager()
{
    if (m_pluginManager)
        return;

    m_pluginManager = std::make_unique<PluginMngr>(m_SPModScriptsDir);
}

void SPGlobal::setScriptsDir(std::string_view folder)
{
    fs::path pathToScripts(m_SPModDir);
    pathToScripts /= folder;
    m_SPModScriptsDir = std::move(pathToScripts);
}

void SPGlobal::setLogsDir(std::string_view folder)
{
    fs::path pathToLogs(m_SPModDir);
    pathToLogs /= folder;
    m_SPModLogsDir = std::move(pathToLogs);
}

#ifdef SP_POSIX
void SPGlobal::setSPFactory(void *library,
                            SourcePawn::ISourcePawnFactory *factory)
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

void SPGlobal::initDefaultsForwards()
{
    using et = IForward::ExecType;
    using param = IForward::ParamType;
    auto paramsList = { param::CELL, param::STRING, param::STRING, param::STRINGEX };
    m_forwardManager->createForwardCore("OnClientConnect", et::STOP, paramsList);
}
