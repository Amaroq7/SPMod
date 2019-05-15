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
                                        m_forwardManager(std::make_unique<ForwardMngr>()),
                                        m_cvarManager(std::make_unique<CvarMngr>()),
                                        m_loggingSystem(std::make_unique<LoggerMngr>()),
                                        m_cmdManager(std::make_unique<CommandMngr>()),
                                        m_timerManager(std::make_unique<TimerMngr>()),
                                        m_menuManager(std::make_unique<MenuMngr>()),
                                        m_plrManager(std::make_unique<PlayerMngr>()),
                                        m_utils(std::make_unique<Utils>()),
                                        m_modName(GET_GAME_INFO(PLID, GINFO_NAME))
{
    // Sets default dirs
    setPluginsDir("plugins");
    setLogsDir("logs");
    setDllsDir("dlls");
    setExtDir("exts");
}

void SPGlobal::setPluginsDir(std::string_view folder)
{
    m_SPModPluginsDir = m_SPModDir / folder.data();
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
    std::shared_ptr<Logger> logger = m_loggingSystem->getLoggerCore("SPMOD");

    if (errCode)
    {
        logger->logToBothCore(LogType::Error, "Can't read extensions directory: ", errCode.message());
        return 0u;
    }

    /* Querying extensions */
    for (const auto &entry : directoryIter)
    {
        const fs::path &extPath = entry.path();
        std::unique_ptr<Extension> extHandle;
        try
        {
            extHandle = std::make_unique<Extension>(extPath);
        }
        catch (const std::runtime_error &e)
        {
            logger->logToBothCore(LogType::Error, e.what());
            continue;
        }

        if (!extHandle->getQueryFunc())
        {
            logger->logToBothCore(LogType::Error, "Querying problem: ", extPath.filename());
            continue;
        }

        ExtQueryValue result = extHandle->getQueryFunc()(gSPGlobal.get());
        if (result == ExtQueryValue::DontLoad)
        {
            logger->logToBothCore(LogType::Error, "Can't be loaded: ", extPath.filename());
            continue;
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

    m_extHandles.clear();
}

bool SPGlobal::canPluginsPrecache() const
{
    return m_canPluginsPrecache;
}

IPlugin *SPGlobal::getPlugin(const char *pluginname) const
{
    if (strlen(pluginname) < 4)
    {
        return nullptr;
    }

    std::string_view pluginExt(pluginname);
    pluginExt = pluginExt.substr(pluginExt.length() - 4);
    for (const auto &interface : m_interfaces)
    {
        IPluginMngr *pluginMngr = interface.second->getPluginMngr();

        if (pluginExt != pluginMngr->getPluginsExt())
            continue;

        return pluginMngr->getPlugin(pluginname);
    }

    return nullptr;
}

void SPGlobal::allowPrecacheForPlugins(bool allow)
{
    m_canPluginsPrecache = allow;
}

const char *SPGlobal::getPath(DirType type) const
{
#if defined SP_POSIX
    return getPathCore(type).c_str();
#else
    static std::string tempDir;
    tempDir = getPathCore(type).string();
    return tempDir.c_str();
#endif
}

const char *SPGlobal::getModName() const
{
    return getModNameCore().data();
}

IForwardMngr *SPGlobal::getForwardManager() const
{
    return getForwardManagerCore().get();
}

ICvarMngr *SPGlobal::getCvarManager() const
{
    return getCvarManagerCore().get();
}

ITimerMngr *SPGlobal::getTimerManager() const
{
    return getTimerManagerCore().get();
}

IMenuMngr *SPGlobal::getMenuManager() const
{
    return m_menuManager.get();
}

ILoggerMngr *SPGlobal::getLoggerManager() const
{
    return getLoggerManagerCore().get();
}

IPlayerMngr *SPGlobal::getPlayerManager() const
{
    return m_plrManager.get();
}

IUtils *SPGlobal::getUtils() const
{
    return getUtilsCore().get();
}

INativeProxy *SPGlobal::getNativeProxy() const
{
    return getNativeProxyCore().get();
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

fs::path SPGlobal::getPathCore(DirType type) const
{
    switch(type)
    {
        case DirType::Home: return m_SPModDir;
        case DirType::Dlls: return m_SPModDllsDir;
        case DirType::Exts: return m_SPModExtsDir;
        case DirType::Logs: return m_SPModLogsDir;
        case DirType::Plugins: return m_SPModPluginsDir;
        default: return {};
    }
}

std::string_view SPGlobal::getModNameCore() const
{
    return m_modName;
}

const std::unique_ptr<ForwardMngr> &SPGlobal::getForwardManagerCore() const
{
    return m_forwardManager;
}

const std::unique_ptr<CvarMngr> &SPGlobal::getCvarManagerCore() const
{
    return m_cvarManager;
}

const std::unique_ptr<LoggerMngr> &SPGlobal::getLoggerManagerCore() const
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

const std::unique_ptr<NativeProxy> &SPGlobal::getNativeProxyCore() const
{
    return m_nativeProxy;
}