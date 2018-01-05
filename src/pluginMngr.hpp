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

#include <spmod.hpp>

class Plugin;

class PluginMngr final : public IPluginMngr
{
public:
    PluginMngr(const fs::path &pathToScripts);
    ~PluginMngr() = default;

    // IPluginMngr
    IPlugin *loadPlugin(const char *name, char *error, size_t size) override;
    void unloadPlugin(const char *name) override;
    IPlugin *getPlugin(size_t index) override;
    IPlugin *getPlugin(const char *name) override;
    size_t getPluginsNum() const override
    {
        return m_plugins.size();
    }

    // PluginMngr
    bool loadPluginFs(const fs::path &path, std::string &error);
    const auto &getPluginsList() const
    {
        return m_plugins;
    }
    void detachPlugins();
    size_t loadPlugins();

private:
    std::unordered_map<std::string, std::shared_ptr<Plugin>> m_plugins;
    fs::path m_scriptsPath;
};
