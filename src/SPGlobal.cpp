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
    setExtDir("exts");

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

void SPGlobal::setExtDir(std::string_view folder)
{
    m_SPModExtsDir = m_SPModDir / folder.data();
}

std::size_t SPGlobal::loadExts()
{
    std::error_code errCode;
    auto directoryIter = fs::directory_iterator(m_SPModExtsDir, errCode);

    if (errCode)
    {
        m_loggingSystem->LogMessageCore("Can't read extensions directory: ", errCode.message());
        return 0u;
    }

    /* Querying extensions */
    for (const auto &entry : directoryIter)
    {
        fs::path extPath = entry.path();
        std::unique_ptr<Extension> extHandle;
        try
        {
            extHandle = std::make_unique<Extension>(entry.path());
        }
        catch (const std::runtime_error &e)
        {
            m_loggingSystem->LogMessageCore(e.what());
            continue;
        }

        if (!extHandle->getQueryFunc())
        {
            m_loggingSystem->LogMessageCore("Querying problem: ", extPath.filename());
            continue;
        }

        ExtQueryValue result = extHandle->getQueryFunc()(gSPGlobal.get());
        if (result == ExtQueryValue::DontLoad)
        {
            m_loggingSystem->LogMessageCore("Can't be loaded: ", extPath.filename());
            continue;
        }

        if (result == ExtQueryValue::MetaExt)
        {
            void *metaHandle = nullptr;

            if (LOAD_PLUGIN(PLID, extPath.string().c_str(), PT_ANYTIME, &metaHandle) || !metaHandle)
            {
                m_loggingSystem->LogMessageCore("Can't attach module: ", extPath.string());
                continue;
            }
            extHandle->setMetaHandle(metaHandle);
        }

        m_extHandles.push_back(std::move(extHandle));
    }

    /* Initialize extensions */
    auto iter = m_extHandles.begin();
    while (iter != m_extHandles.end())
    {
        std::unique_ptr<Extension> &ext = *iter;
        if (!ext->getInitFunc() || !ext->getInitFunc()())
        {
            iter = m_extHandles.erase(iter);
        }
        else
            ++iter;
    }

    return m_extHandles.size();
}

void SPGlobal::unloadExts()
{
    for (auto &ext : m_extHandles)
    {
        if (ext->getEndFunc())
            ext->getEndFunc()();
    }

    for (auto &ext : m_extHandles)
    {
        if (ext->metaHandle())
            UNLOAD_PLUGIN_BY_HANDLE(PLID, ext->metaHandle(), PT_ANYTIME, PNL_PLUGIN);
    }

    m_extHandles.clear();
}

void SPGlobal::_initSourcePawn()
{
    fs::path SPDir(getDllsDirCore());
    SPDir /= SPGlobal::sourcepawnLibrary;

#ifdef SP_POSIX
    void *libraryHandle = dlopen(SPDir.c_str(), RTLD_NOW | RTLD_LOCAL | RTLD_DEEPBIND);
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

const char *SPGlobal::getHomeDir() const
{
#if defined SP_POSIX
    return getHomeDirCore().c_str();
#else
    static std::string tempDir;
    tempDir = getHomeDirCore().string();
    return tempDir.c_str();
#endif
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
    return m_interfaces.try_emplace(interface->getName(), interface).second;
}

IInterface *SPGlobal::getInterface(const char *name) const
{
    if (auto iter = m_interfaces.find(name); iter != m_interfaces.end())
        return iter->second;

    return nullptr;
}

fs::path SPGlobal::getHomeDirCore() const
{
    return m_SPModDir;
}

std::string_view SPGlobal::getModNameCore() const
{
    return m_modName;
}

const std::unique_ptr<PluginMngr> &SPGlobal::getPluginManagerCore() const
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

const std::unique_ptr<MenuMngr> &SPGlobal::getMenuManagerCore() const
{
    return m_menuManager;
}

const std::unique_ptr<PlayerMngr> &SPGlobal::getPlayerManagerCore() const
{
    return m_plrManager;
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
