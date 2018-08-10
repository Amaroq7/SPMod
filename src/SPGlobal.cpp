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
    return getHomeCore.data();
}

const char *SPGlobal::getModName() const
{
    return getModNameCore().data();
}

IPluginMngr *SPGlobal::getPluginManager() const
{
    return getPluginManagerCore().get();
}

IForwardMngr *SPGlobal::getForwardManager() const
{
    return getForwardManagerCore().get();
}

ICvarMngr *SPGlobal::getCvarManager() const
{
    return getCvarManagerCore().get();
}

SourcePawn::ISourcePawnEnvironment *SPGlobal::getSPEnvironment() const
{
    return m_spFactory->CurrentEnvironment();
}

ITimerMngr *SPGlobal::getTimerManager() const
{
    return getTimerManagerCore().get();
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
    return getUtilsCore().get();
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

std::string_view SPGlobal::getHomeCore() const
{
    return m_SPModDir.string();
}

std::string_view SPGlobal::getModNameCore() const
{
    return m_modName;
}

const std::unqiue_ptr<PluginMngr> &SPGlobal::getPluginManagerCore() const
{
    return m_pluginManager;
}
const std::unique_ptr<ForwardMngr> &SPGlobal::getForwardManagerCore() const
{
    return m_forwardManager;
}
const std::unique_ptr<CvarMngr> &SPGlobal::getCvarManagerCore() const
{
    return m_cvarManager;
}
const std::unique_ptr<Logger> &SPGlobal::getLoggerCore() const
{
    return m_loggingSystem;
}
const std::unique_ptr<CommandMngr> &SPGlobal::getCommandManagerCore() const
{
    return m_cmdManager;
}
const std::unique_ptr<TimerMngr> &SPGlobal::getTimerManagerCore() const
{
    return m_timerManager;
}
const std::unique_ptr<Utils> &SPGlobal::getUtilsCore() const
{
    return m_utils;
}
const fs::path &SPGlobal::getScriptsDirCore() const
{
    return m_SPModScriptsDir;
}
const fs::path &SPGlobal::getLogsDirCore() const
{
    return m_SPModLogsDir;
}
const fs::path &SPGlobal::getDllsDirCore() const
{
    return m_SPModDllsDir;
}
