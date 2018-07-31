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

class PluginMngr;
class ForwardMngr;
class CvarMngr;
class Logger;

class SPGlobal : public ISPGlobal
{
public:
    #ifdef SP_LINUX
        static constexpr auto *sourcepawnLibrary = "sourcepawn.jit.x86.so";
    #elif defined SP_WINDOWS
        static constexpr auto *sourcepawnLibrary = "sourcepawn.jit.x86.dll";
    #endif

    SPGlobal() = delete;
    explicit SPGlobal(fs::path &&dllDir);
    ~SPGlobal()
    {
        #ifdef SP_POSIX
            dlclose(m_SPLibraryHandle);
        #else
            FreeLibrary(m_SPLibraryHandle);
        #endif
    }

    // ISPGlobal
    const char *getHome() const override;
    const char *getModName() const override;
    IPluginMngr *getPluginManager() const override;
    IForwardMngr *getForwardManager() const override;
    ICvarMngr *getCvarManager() const override;
    SourcePawn::ISourcePawnEnvironment *getSPEnvironment() const override;
    ITimerMngr *getTimerManager() const override;
    IMenuMngr *getMenuManager() const override;
    IPlayerMngr *getPlayerManager() const override;
    IUtils *getUtils() const override;
    bool registerInterface(IInterface *interface) override;
    IInterface *getInterface(const char *name) const override;

    unsigned int formatString(char *buffer,
                              std::size_t length,
                              const char *format,
                              SourcePawn::IPluginContext *ctx,
                              const cell_t *params,
                              std::size_t param) const override;

    // SPGlobal
    const auto &getPluginManagerCore() const
    {
        return m_pluginManager;
    }
    const auto &getForwardManagerCore() const
    {
        return m_forwardManager;
    }
    const auto &getCvarManagerCore() const
    {
        return m_cvarManager;
    }
    const auto &getLoggerCore() const
    {
        return m_loggingSystem;
    }
    const auto &getCommandManagerCore() const
    {
        return m_cmdManager;
    }
    const auto &getTimerManagerCore() const
    {
        return m_timerManager;
    }
    const auto &getMenuManagerCore() const
    {
        return m_menuManager;
    }
    const auto &getUtilsCore() const
    {
        return m_utils;
    }
    const std::unique_ptr<PlayerMngr> &getPlayerManagerCore() const
    {
        return m_plrManager;
    }
    const auto &getScriptsDirCore()
    {
        return m_SPModScriptsDir;
    }
    const auto &getLogsDirCore() const
    {
        return m_SPModLogsDir;
    }
    const auto &getDllsDirCore() const
    {
        return m_SPModDllsDir;
    }
    const auto &getInterfacesList() const
    {
        return m_interfaces;
    }

    void setScriptsDir(std::string_view folder);
    void setLogsDir(std::string_view folder);
    void setDllsDir(std::string_view folder);

private:
    void _initSourcePawn();

    fs::path m_SPModScriptsDir;
    fs::path m_SPModDir;
    fs::path m_SPModLogsDir;
    fs::path m_SPModDllsDir;
    std::unique_ptr<PluginMngr> m_pluginManager;
    std::unique_ptr<ForwardMngr> m_forwardManager;
    std::unique_ptr<CvarMngr> m_cvarManager;
    std::unique_ptr<Logger> m_loggingSystem;
    std::unique_ptr<CommandMngr> m_cmdManager;
    std::unique_ptr<TimerMngr> m_timerManager;
    std::unique_ptr<MenuMngr> m_menuManager;
    std::unique_ptr<PlayerMngr> m_plrManager;
    std::unique_ptr<Utils> m_utils;
    std::string m_modName;
    SourcePawn::ISourcePawnFactory *m_spFactory;
    std::unordered_map<std::string, IInterface *> m_interfaces;

    // SourcePawn library handle
#ifdef SP_POSIX
    void *m_SPLibraryHandle;
#else
    HMODULE m_SPLibraryHandle;
#endif
};

extern std::unique_ptr<SPGlobal> gSPGlobal;