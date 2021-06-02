/*
 *  Copyright (C) 2018-2020 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  SPMod is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with SPMod.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "SPGlobal.hpp"
#include <SPConfig.hpp>
#include "MetaInit.hpp"

std::unique_ptr<SPGlobal> gSPGlobal;

SPGlobal::SPGlobal(fs::path &&dllDir)
    : m_SPModDir(dllDir.parent_path().parent_path()), m_forwardManager(std::make_unique<ForwardMngr>()),
      m_loggingSystem(std::make_unique<LoggerMngr>()), m_cmdManager(std::make_unique<CommandMngr>()),
      m_timerManager(std::make_unique<TimerMngr>()), m_menuManager(std::make_unique<MenuMngr>()),
      m_messageManager(std::make_unique<MessageMngr>()), m_plrManager(std::make_unique<PlayerMngr>()),
      m_nativeProxy(std::make_unique<NativeProxy>()), m_utils(std::make_unique<Utils>())
{
    // Sets default dirs
    setPath(DirType::Plugins, "plugins");
    setPath(DirType::Logs, "logs");
    setPath(DirType::Dlls, "dlls");
    setPath(DirType::Exts, "exts");
    setPath(DirType::Configs, "configs");
}

std::size_t SPGlobal::loadExts()
{
    using namespace std::string_literals;
    std::error_code errCode;
    auto directoryIter = fs::directory_iterator(m_SPModExtsDir, errCode);
    auto logger = m_loggingSystem->getLogger(gSPModLoggerName);

    if (errCode)
    {
        logger->logToBoth(LogLevel::Error, "Can't read extensions directory: " + errCode.message());
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
            logger->logToBoth(LogLevel::Error, e.what());
            continue;
        }

        if (!extHandle->getQueryFunc())
        {
            logger->logToBoth(LogLevel::Error, "Querying problem: " + extPath.filename().string());
            continue;
        }

        ExtQueryValue result = extHandle->getQueryFunc()(gSPGlobal.get());
        if (result == ExtQueryValue::DontLoad)
        {
            logger->logToBoth(LogLevel::Error, "Can't be loaded: " + extPath.filename().string());
            continue;
        }

        m_extHandles.emplace_back(std::move(extHandle));
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

    m_adaptersInterfaces.clear();
    m_modulesInterfaces.clear();
    m_extHandles.clear();
}

bool SPGlobal::canPluginsPrecache() const
{
    return m_canPluginsPrecache;
}

IPlugin *SPGlobal::getPlugin(std::string_view pluginname) const
{
    if (pluginname.length() < 4)
    {
        return nullptr;
    }

    std::string_view pluginExt(pluginname);
    pluginExt = pluginExt.substr(pluginExt.length() - 4);
    for (const auto &interface : m_adaptersInterfaces)
    {
        auto pluginMngr = interface.second->getPluginMngr();

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

const fs::path &SPGlobal::getPath(DirType type) const
{
    static fs::path empty;
    switch (type)
    {
        case DirType::Home:
            return m_SPModDir;
        case DirType::Dlls:
            return m_SPModDllsDir;
        case DirType::Exts:
            return m_SPModExtsDir;
        case DirType::Logs:
            return m_SPModLogsDir;
        case DirType::Plugins:
            return m_SPModPluginsDir;
        case DirType::Configs:
            return m_SPModConfigsDir;
        default:
            return empty;
    }
}

void SPGlobal::setPath(DirType type, std::string_view path)
{
    switch (type)
    {
        case DirType::Home:
            break;
        case DirType::Dlls:
        {
            m_SPModDllsDir = m_SPModDir / path;
            break;
        }
        case DirType::Exts:
        {
            m_SPModExtsDir = m_SPModDir / path;
            break;
        }
        case DirType::Logs:
        {
            m_SPModLogsDir = m_SPModDir / path;
            break;
        }
        case DirType::Plugins:
        {
            m_SPModPluginsDir = m_SPModDir / path;
            break;
        }
        case DirType::Configs:
        {
            m_SPModConfigsDir = m_SPModDir / path;
        }
        default:
            break;
    }
}

ForwardMngr *SPGlobal::getForwardManager() const
{
    return m_forwardManager.get();
}

TimerMngr *SPGlobal::getTimerManager() const
{
    return m_timerManager.get();
}

MenuMngr *SPGlobal::getMenuManager() const
{
    return m_menuManager.get();
}

MessageMngr *SPGlobal::getMessageManager() const
{
    return m_messageManager.get();
}

LoggerMngr *SPGlobal::getLoggerManager() const
{
    return m_loggingSystem.get();
}

PlayerMngr *SPGlobal::getPlayerManager() const
{
    return m_plrManager.get();
}

CommandMngr *SPGlobal::getCommandManager() const
{
    return m_cmdManager.get();
}

Utils *SPGlobal::getUtils() const
{
    return m_utils.get();
}

NativeProxy *SPGlobal::getNativeProxy() const
{
    return m_nativeProxy.get();
}

bool SPGlobal::registerAdapter(IAdapterInterface *interface)
{
    return m_adaptersInterfaces.try_emplace(interface->getName().data(), interface).second;
}

bool SPGlobal::registerModule(IModuleInterface *interface)
{
    return m_modulesInterfaces.try_emplace(interface->getName().data(), interface).second;
}

IModuleInterface *SPGlobal::getInterface(std::string_view name) const
{
    if (auto iter = m_modulesInterfaces.find(name.data()); iter != m_modulesInterfaces.end())
        return iter->second;

    return nullptr;
}

void SPGlobal::setUserMsgId(UserMsgId msgid, Metamod::Engine::MsgType userMsgId)
{
    m_userMsgs[static_cast<std::underlying_type_t<UserMsgId>>(msgid)] = userMsgId;
}

Metamod::Engine::MsgType SPGlobal::getUserMsgId(UserMsgId msgid) const
{
    return m_userMsgs[static_cast<std::underlying_type_t<UserMsgId>>(msgid)];
}

Hooks *SPGlobal::getHooks() const
{
    return m_hooks.get();
}
