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
#include "AccessSystem.hpp"
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

    explicit SPGlobal(std::filesystem::path &&dllDir);

    // ISPGlobal
    const std::filesystem::path &getPath(DirType type) const final;
    nstd::observer_ptr<IPlugin> getPlugin(std::string_view pluginname) const final;

    nstd::observer_ptr<IForwardMngr> getForwardManager() const final;
    nstd::observer_ptr<ITimerMngr> getTimerManager() const final;
    nstd::observer_ptr<IMenuMngr> getMenuManager() const final;
    nstd::observer_ptr<IMessageMngr> getMessageManager() const final;
    nstd::observer_ptr<ILoggerMngr> getLoggerManager() const final;
    nstd::observer_ptr<IPlayerMngr> getPlayerManager() const final;
    nstd::observer_ptr<INativeProxy> getNativeProxy() const final;
    nstd::observer_ptr<ICommandMngr> getCommandManager() const final;
    nstd::observer_ptr<IGroupMngr> getGroupAccessManager() const final;
    nstd::observer_ptr<IUtils> getUtils() const final;
    nstd::observer_ptr<IHooks> getHooks() const final;

    bool registerModule(nstd::observer_ptr<IModuleInterface> interface) final;
    bool registerAdapter(nstd::observer_ptr<IAdapterInterface> interface) final;
    nstd::observer_ptr<IModuleInterface> getInterface(std::string_view name) const override;
    void setUserMsgId(UserMsgId msgId, Anubis::Engine::MsgType userMsgId);
    Anubis::Engine::MsgType getUserMsgId(UserMsgId msgId) const;

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

private:
    std::filesystem::path m_SPModDir;
    std::filesystem::path m_SPModPluginsDir;
    std::filesystem::path m_SPModLogsDir;
    std::filesystem::path m_SPModDllsDir;
    std::filesystem::path m_SPModExtsDir;
    std::filesystem::path m_SPModConfigsDir;

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
    std::shared_ptr<GroupMngr> m_accessGroupMngr;

    std::unordered_map<std::string, nstd::observer_ptr<IModuleInterface>> m_modulesInterfaces;
    std::unordered_map<std::string, nstd::observer_ptr<IAdapterInterface>> m_adaptersInterfaces;
    std::vector<std::unique_ptr<Extension>> m_extHandles;
    std::array<Anubis::Engine::MsgType, MAX_USER_MESSAGES> m_userMsgs;
};

extern std::unique_ptr<SPGlobal> gSPGlobal;
