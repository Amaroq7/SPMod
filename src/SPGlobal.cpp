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
                                        m_cvarManager(std::make_unique<CvarMngr>()),
                                        m_loggingSystem(std::make_unique<Logger>()),
                                        m_cmdManager(std::make_unique<CommandMngr>()),
                                        m_timerManager(std::make_unique<TimerMngr>()),
                                        m_menuManager(std::make_unique<MenuMngr>()),
                                        m_plrManager(std::make_unique<PlayerMngr>()),
                                        m_utils(std::make_unique<Utils>()),
                                        m_modName(GET_GAME_INFO(PLID, GINFO_NAME)),
                                        m_spFactory(nullptr)
{
    // Sets default dirs
    setScriptsDir("scripts");
    setLogsDir("logs");
    setDllsDir("dlls");

    // Initialize SourcePawn library
    _initSourcePawn();

    // Sets up listener for debbugging
    getSPEnvironment()->APIv2()->SetDebugListener(m_loggingSystem.get());
}

void SPGlobal::setScriptsDir(std::string_view folder)
{
    m_SPModScriptsDir = m_SPModDir / folder.data();
}

void SPGlobal::setLogsDir(std::string_view folder)
{
    m_SPModLogsDir = m_SPModDir / folder.data();
}

void SPGlobal::setDllsDir(std::string_view folder)
{
    m_SPModDllsDir = m_SPModDir / folder.data();
}

void SPGlobal::_initSourcePawn()
{
    fs::path SPDir(getDllsDirCore());
    SPDir /= SPGlobal::sourcepawnLibrary;

#ifdef SP_POSIX
    void *libraryHandle = dlopen(SPDir.c_str(), RTLD_NOW);
#else
    HMODULE libraryHandle = LoadLibrary(SPDir.string().c_str());
#endif

    if (!libraryHandle)
        throw std::runtime_error("Failed to open SourcePawn library");

#ifdef SP_POSIX
    auto getFactoryFunc = reinterpret_cast<SourcePawn::GetSourcePawnFactoryFn>
                                (dlsym(libraryHandle, "GetSourcePawnFactory"));
#else
    auto getFactoryFunc = reinterpret_cast<SourcePawn::GetSourcePawnFactoryFn>
                                (GetProcAddress(libraryHandle, "GetSourcePawnFactory"));
#endif

    if (!getFactoryFunc)
    {
#ifdef SP_POSIX
        dlclose(libraryHandle);
#else
        FreeLibrary(libraryHandle);
#endif
        throw std::runtime_error("Cannot find SourcePawn factory function");
    }

    SourcePawn::ISourcePawnFactory *SPFactory = getFactoryFunc(SOURCEPAWN_API_VERSION);
    if (!SPFactory)
    {
#ifdef SP_POSIX
        dlclose(libraryHandle);
#else
        FreeLibrary(libraryHandle);
#endif
        throw std::runtime_error("Wrong SourcePawn library version");
    }

    m_SPLibraryHandle = libraryHandle;
    m_spFactory = SPFactory;
    m_spFactory->NewEnvironment();
    getSPEnvironment()->APIv2()->SetJitEnabled(true);
}

const char *SPGlobal::getHome() const
{
    return m_SPModDir.string().c_str();
}

const char *SPGlobal::getModName() const
{
    return m_modName.c_str();
}

IPluginMngr *SPGlobal::getPluginManager() const
{
    return m_pluginManager.get();
}

IForwardMngr *SPGlobal::getForwardManager() const
{
    return m_forwardManager.get();
}

ICvarMngr *SPGlobal::getCvarManager() const
{
    return m_cvarManager.get();
}

SourcePawn::ISourcePawnEnvironment *SPGlobal::getSPEnvironment() const
{
    return m_spFactory->CurrentEnvironment();
}

ITimerMngr *SPGlobal::getTimerManager() const
{
    return m_timerManager.get();
}

IMenuMngr *SPGlobal::getMenuManager() const
{
    return m_menuManager.get();
}

IPlayerMngr *SPGlobal::getPlayerManager() const
{
    return m_plrManager.get();
}

IUtils *SPGlobal::getUtils() const
{
    return m_utils.get();
}

bool SPGlobal::registerInterface(IInterface *interface)
{
    return m_interfaces.try_emplace(interface->getInterfaceName(), interface).second;
}

IInterface *SPGlobal::getInterface(const char *name) const
{
    if (auto iter = m_interfaces.find(name); iter != m_interfaces.end())
        return iter->second;

    return nullptr;
}