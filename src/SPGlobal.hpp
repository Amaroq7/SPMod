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

    IForwardMngr *getForwardManager() const override;
    ICvarMngr *getCvarManager() const override;
    ITimerMngr *getTimerManager() const override;
    IMenuMngr *getMenuManager() const override;
    ILoggerMngr *getLoggerManager() const override;
    IPlayerMngr *getPlayerManager() const override;
    IUtils *getUtils() const override;

    bool registerInterface(IInterface *interface) override;
    IInterface *getInterface(const char *name) const override;

    // SPGlobal
    fs::path getPathCore(DirType type) const;
    std::string_view getModNameCore() const;

    const std::unique_ptr<ForwardMngr> &getForwardManagerCore() const;
    const std::unique_ptr<CvarMngr> &getCvarManagerCore() const;
    const std::unique_ptr<CommandMngr> &getCommandManagerCore() const;
    const std::unique_ptr<TimerMngr> &getTimerManagerCore() const;
    const std::unique_ptr<Utils> &getUtilsCore() const;
    const std::unique_ptr<MenuMngr> &getMenuManagerCore() const;
    const std::unique_ptr<LoggerMngr> &getLoggerManagerCore() const;
    const std::unique_ptr<PlayerMngr> &getPlayerManagerCore() const;
    const auto &getInterfacesList() const
    {
        return m_interfaces;
    }

    void setPluginsDir(std::string_view folder);
    void setLogsDir(std::string_view folder);
    void setDllsDir(std::string_view folder);
    void setExtDir(std::string_view folder);

    std::size_t loadExts();
    void unloadExts();

    bool canPluginsPrecache() const;
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
    std::unique_ptr<Utils> m_utils;

    std::string m_modName;
    std::unordered_map<std::string, IInterface *> m_interfaces;
    std::vector<std::unique_ptr<Extension>> m_extHandles;

    bool m_canPluginsPrecache;
};

extern std::unique_ptr<SPGlobal> gSPGlobal;
