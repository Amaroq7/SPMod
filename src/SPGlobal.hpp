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
class Logger;

class SPGlobal : public ISPGlobal
{
public:

    static constexpr auto *sourcepawnLibrary = "sourcepawn.jit.x86.so";

    SPGlobal() = delete;
    SPGlobal(fs::path &&dllDir);
    ~SPGlobal()
    {
        #ifdef SP_POSIX
            dlclose(m_SPLibraryHandle);
        #else
            // TODO: windows
        #endif
    }

    // ISPGlobal
    const char *getHome() const override
    {
        return m_SPModDir.c_str();
    }
    const char *getModName() const override
    {
        return m_modName.c_str();
    }
    IPluginMngr *getPluginManager() const override
    {
        return reinterpret_cast<IPluginMngr *>(m_pluginManager.get());
    }
    IForwardMngr *getForwardManager() const override
    {
        return reinterpret_cast<IForwardMngr *>(m_forwardManager.get());
    }
    SourcePawn::ISourcePawnEnvironment *getSPEnvironment() const override
    {
        return m_spFactory->CurrentEnvironment();
    }
    bool addModule(IModuleInterface *interface) override;

    // SPGlobal
    const std::unique_ptr<PluginMngr> &getPluginManagerCore() const
    {
        return m_pluginManager;
    }
    const std::unique_ptr<ForwardMngr> &getForwardManagerCore() const
    {
        return m_forwardManager;
    }
    const std::unique_ptr<Logger> &getLoggerCore() const
    {
        return m_loggingSystem;
    }
    const auto &getModulesList() const
    {
        return m_modulesNames;
    }
    const auto &getScriptsDirCore()
    {
        return m_SPModScriptsDir;
    }

    void setScriptsDir(std::string_view folder);
    void setLogsDir(std::string_view folder);

private:

    void _initSourcePawn();

    struct NativeDef
    {
        sp_nativeinfo_t *natives;
        size_t num;
    };

    fs::path m_SPModScriptsDir;
    fs::path m_SPModDir;
    fs::path m_SPModLogsDir;
    std::unique_ptr<PluginMngr> m_pluginManager;
    std::unique_ptr<ForwardMngr> m_forwardManager;
    std::unique_ptr<Logger> m_loggingSystem;
    std::string m_modName;
    std::unordered_map<std::string, NativeDef> m_modulesNames;
    SourcePawn::ISourcePawnFactory *m_spFactory;

    // SourcePawn library handle
#ifdef SP_POSIX
    void *m_SPLibraryHandle;
#else
    // TODO: windows
#endif
};

extern std::unique_ptr<SPGlobal> gSPGlobal;
