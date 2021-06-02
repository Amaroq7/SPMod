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

#pragma once

#include <ISPGlobal.hpp>
#include <SPConfig.hpp>
#include <public/IMetamod.hpp>
#include "MenuSystem.hpp"
#include "ForwardSystem.hpp"
#include "TimerSystem.hpp"
#include "MessageSystem.hpp"
#include "LoggingSystem.hpp"
#include "PlayerSystem.hpp"
#include "NativeProxy.hpp"
#include "CmdSystem.hpp"
#include "UtilsSystem.hpp"
#include "ExtensionSystem.hpp"
#include "Hooks.hpp"

using namespace SPMod;

enum class UserMsgId : std::uint8_t
{
    ShowMenu = 0,
    VGUIMenu,
    TextMsg
};

class SPGlobal final : public ISPGlobal
{
public:
    SPGlobal() = delete;
    SPGlobal(const SPGlobal &other) = delete;
    SPGlobal(SPGlobal &&other) = default;
    ~SPGlobal() = default;

    explicit SPGlobal(fs::path &&dllDir);

    // ISPGlobal
    const fs::path &getPath(DirType type) const override;
    bool canPluginsPrecache() const override;
    IPlugin *getPlugin(std::string_view pluginname) const override;

    ForwardMngr *getForwardManager() const override;
    TimerMngr *getTimerManager() const override;
    MenuMngr *getMenuManager() const override;
    MessageMngr *getMessageManager() const override;
    LoggerMngr *getLoggerManager() const override;
    PlayerMngr *getPlayerManager() const override;
    NativeProxy *getNativeProxy() const override;
    CommandMngr *getCommandManager() const override;
    Utils *getUtils() const override;
    Hooks *getHooks() const override;

    bool registerModule(IModuleInterface *interface) override;
    bool registerAdapter(IAdapterInterface *interface) override;
    IModuleInterface *getInterface(std::string_view name) const override;
    void setUserMsgId(UserMsgId msgid, Metamod::Engine::MsgType userMsgId);
    Metamod::Engine::MsgType getUserMsgId(UserMsgId msgid) const;

    // SPGlobal
    const auto &getModulesInterfaces() const
    {
        return m_modulesInterfaces;
    }
    const auto &getAdaptersInterfaces() const
    {
        return m_adaptersInterfaces;
    }

    void setPath(DirType type, std::string_view path);

    std::size_t loadExts();
    void unloadExts();

    void allowPrecacheForPlugins(bool allow);

private:
    fs::path m_SPModDir;
    fs::path m_SPModPluginsDir;
    fs::path m_SPModLogsDir;
    fs::path m_SPModDllsDir;
    fs::path m_SPModExtsDir;
    fs::path m_SPModConfigsDir;

    std::unique_ptr<ForwardMngr> m_forwardManager;
    std::unique_ptr<LoggerMngr> m_loggingSystem;
    std::unique_ptr<CommandMngr> m_cmdManager;
    std::unique_ptr<TimerMngr> m_timerManager;
    std::unique_ptr<MenuMngr> m_menuManager;
    std::unique_ptr<MessageMngr> m_messageManager;
    std::unique_ptr<PlayerMngr> m_plrManager;
    std::unique_ptr<NativeProxy> m_nativeProxy;
    std::unique_ptr<Utils> m_utils;
    std::unique_ptr<Hooks> m_hooks;

    std::unordered_map<std::string, IModuleInterface *> m_modulesInterfaces;
    std::unordered_map<std::string, IAdapterInterface *> m_adaptersInterfaces;
    std::vector<std::unique_ptr<Extension>> m_extHandles;
    std::array<Metamod::Engine::MsgType, MAX_USER_MESSAGES> m_userMsgs;

    bool m_canPluginsPrecache;
};

extern std::unique_ptr<SPGlobal> gSPGlobal;
