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

#include "spmod.hpp"

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
    std::string_view getModName() const override;
    bool canPluginsPrecache() const override;
    IPlugin *getPlugin(std::string_view pluginname) const override;

    ForwardMngr *getForwardManager() const override;
    CvarMngr *getCvarManager() const override;
    TimerMngr *getTimerManager() const override;
    MenuMngr *getMenuManager() const override;
    MessageMngr *getMessageManager() const override;
    LoggerMngr *getLoggerManager() const override;
    PlayerMngr *getPlayerManager() const override;
    NativeProxy *getNativeProxy() const override;
    CommandMngr *getCommandManager() const override;
    EngineFuncs *getEngineFuncs() const override;
    EngineFuncsHooked *getEngineHookedFuncs() const override;
    EngineGlobals *getEngineGlobals() const override;
    MetaFuncs *getMetaFuncs() const override;
    Edict *getEdict(std::uint32_t index) override;
    Utils *getUtils() const override;

    bool registerModule(IModuleInterface *interface) override;
    bool registerAdapter(IAdapterInterface *interface) override;
    IModuleInterface *getInterface(std::string_view name) const override;

    // SPGlobal
    void clearEdicts();
    void removeEdict(edict_t *edict);
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

    std::unique_ptr<ForwardMngr> m_forwardManager;
    std::unique_ptr<CvarMngr> m_cvarManager;
    std::unique_ptr<LoggerMngr> m_loggingSystem;
    std::unique_ptr<CommandMngr> m_cmdManager;
    std::unique_ptr<TimerMngr> m_timerManager;
    std::unique_ptr<MenuMngr> m_menuManager;
    std::unique_ptr<MessageMngr> m_messageManager;
    std::unique_ptr<PlayerMngr> m_plrManager;
    std::unique_ptr<NativeProxy> m_nativeProxy;
    std::unique_ptr<EngineFuncs> m_engineFuncs;
    std::unique_ptr<EngineFuncsHooked> m_engineFuncsHooked;
    std::unique_ptr<EngineGlobals> m_engineGlobals;
    std::unique_ptr<MetaFuncs> m_metaFuncs;
    std::unique_ptr<Utils> m_utils;

    std::string m_modName;
    std::unordered_map<std::string, IModuleInterface *> m_modulesInterfaces;
    std::unordered_map<std::string, IAdapterInterface *> m_adaptersInterfaces;
    std::vector<std::unique_ptr<Extension>> m_extHandles;
    std::unordered_map<std::uint32_t, std::unique_ptr<Edict>> m_edicts;

    bool m_canPluginsPrecache;
};

extern std::unique_ptr<SPGlobal> gSPGlobal;
