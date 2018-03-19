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

SPGlobal::SPGlobal(fs::path &&dllDir) : m_SPModDir(dllDir.parent_path().parent_path()),
                                        m_pluginManager(std::make_unique<PluginMngr>()),
                                        m_forwardManager(std::make_unique<ForwardMngr>()),
                                        m_loggingSystem(std::make_unique<Logger>()),
                                        m_modName(GET_GAME_INFO(PLID, GINFO_NAME)),
                                        m_spFactory(nullptr)
{
    // Sets default dirs
    setScriptsDir("scripts");
    setLogsDir("logs");

    // Initialize SourcePawn library
    _initSourcePawn();

    // Add definition of core module
    addModule(gSPModModuleDef.get());

    // Sets up listener for debbugging
    getSPEnvironment()->APIv2()->SetDebugListener(m_loggingSystem.get());
}

bool SPGlobal::addModule(IModuleInterface *interface)
{
    //TODO: Error reporting?
    if (interface->getInterfaceVersion() > SPMOD_API_VERSION)
        return false;

    const auto [iter, added] = m_modulesNames.try_emplace(interface->getName(),
                                                            interface->getNatives(),
                                                            interface->getNativesNum());

    return added;
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

void SPGlobal::_initSourcePawn()
{
    fs::path dllsDir(getHome());
    dllsDir /= "dlls";
    dllsDir /= SPGlobal::sourcepawnLibrary;

#ifdef SP_POSIX
    void *libraryHandle = dlopen(dllsDir.c_str(), RTLD_NOW);
#else
    #error Need Windows implementation
#endif

    if (!libraryHandle)
        throw std::runtime_error("Failed to open SourcePawn library");

#ifdef SP_POSIX
    auto getFactoryFunc = reinterpret_cast<SourcePawn::GetSourcePawnFactoryFn>
                                (dlsym(libraryHandle, "GetSourcePawnFactory"));
#else
    //TODO: windows
#endif

    if (!getFactoryFunc)
    {
#ifdef SP_POSIX
        dlclose(libraryHandle);
#else
        //TODO: windows
#endif
        throw std::runtime_error("Cannot find SourcePawn factory function");
    }

    SourcePawn::ISourcePawnFactory *SPFactory = getFactoryFunc(SOURCEPAWN_API_VERSION);
    if (!SPFactory)
    {
#ifdef SP_POSIX
        dlclose(libraryHandle);
#else
        #error Need Windows implementation
#endif
        throw std::runtime_error("Wrong SourcePawn library version");
    }

#ifdef SP_POSIX
    m_SPLibraryHandle = libraryHandle;
#else
    #error Need Windows implementation
#endif
    m_spFactory = SPFactory;
    m_spFactory->NewEnvironment();
    getSPEnvironment()->APIv2()->SetJitEnabled(true);
}
