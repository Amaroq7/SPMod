/*
 *  Copyright (C) 2018 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
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

#pragma once

#include "spmod.hpp"

class ForwardMngr;
class CvarMngr;
class LoggerMngr;

class SPGlobal final : public ISPGlobal
{
public:
    SPGlobal() = delete;
    SPGlobal(const SPGlobal &other) = delete;
    SPGlobal(SPGlobal &&other) = default;
    ~SPGlobal() = default;

    explicit SPGlobal(fs::path &&dllDir);

    // ISPGlobal
    const char *getPath(DirType type) const override;
    const char *getModName() const override;
    bool canPluginsPrecache() const override;
    IPlugin *getPlugin(const char *pluginname) const override;

    IForwardMngr *getForwardManager() const override;
    ICvarMngr *getCvarManager() const override;
    ITimerMngr *getTimerManager() const override;
    IMenuMngr *getMenuManager() const override;
    ILoggerMngr *getLoggerManager() const override;
    IPlayerMngr *getPlayerManager() const override;
    INativeProxy *getNativeProxy() const override;
    ICommandMngr *getCommandManager() const override;
    IEngineFuncs *getEngineFuncs() const override;
    IEngineFuncsHooked *getEngineHookedFuncs() const override;
    IEngineGlobals *getEngineGlobals() const override;
    IMetaFuncs *getMetaFuncs() const override;
    IEdict *getEdict(int index) override;
    IUtils *getUtils() const override;

    bool registerModule(IModuleInterface *interface) override;
    bool registerAdapter(IAdapterInterface *interface) override;
    IModuleInterface *getInterface(const char *name) const override;

    // SPGlobal
    const fs::path &getPathCore(DirType type) const;
    std::string_view getModNameCore() const;

    const std::unique_ptr<ForwardMngr> &getForwardManagerCore() const;
    const std::unique_ptr<CvarMngr> &getCvarManagerCore() const;
    const std::unique_ptr<CommandMngr> &getCommandManagerCore() const;
    const std::unique_ptr<TimerMngr> &getTimerManagerCore() const;
    const std::unique_ptr<Utils> &getUtilsCore() const;
    const std::unique_ptr<MenuMngr> &getMenuManagerCore() const;
    const std::unique_ptr<LoggerMngr> &getLoggerManagerCore() const;
    const std::unique_ptr<PlayerMngr> &getPlayerManagerCore() const;
    const std::unique_ptr<NativeProxy> &getNativeProxyCore() const;
    std::shared_ptr<Edict> getEdictCore(int index);
    void clearEdicts();
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
    std::vector<IPluginMngr *> m_pluginManagers;
    std::unordered_map<int, std::shared_ptr<Edict>> m_edicts;

    bool m_canPluginsPrecache;
};

extern std::unique_ptr<SPGlobal> gSPGlobal;
