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

class Plugin final : public IPlugin
{
public:

    static constexpr uint32_t FIELD_NAME = 0;
    static constexpr uint32_t FIELD_VERSION = 1;
    static constexpr uint32_t FIELD_AUTHOR = 2;
    static constexpr uint32_t FIELD_URL = 3;

    Plugin(std::size_t id,
           std::string_view identity,
           const fs::path &path);

    Plugin() = delete;
    ~Plugin() = default;

    // IPlugin
    const char *getName() const override;
    const char *getVersion() const override;
    const char *getAuthor() const override;
    const char *getUrl() const override;
    const char *getIndentity() const override;
    const char *getFilename() const override;
    std::size_t getId() const override;
    SourcePawn::IPluginRuntime *getRuntime() const override;

    // Plugin
    std::string_view getNameCore() const;
    std::string_view getVersionCore() const;
    std::string_view getAuthorCore() const;
    std::string_view getUrlCore() const;
    std::string_view getIndentityCore() const;
    std::string_view getFileNameCore() const;

private:
    SourcePawn::IPluginRuntime *m_runtime;
    std::string m_identity;
    std::string m_filename;
    std::string m_name;
    std::string m_version;
    std::string m_author;
    std::string m_url;
    std::size_t m_id;
};

class PluginMngr final : public IPluginMngr
{
public:
    PluginMngr() = default;
    ~PluginMngr() = default;

    // IPluginMngr
    std::size_t getPluginsNum() const override;
    IPlugin *getPlugin(std::size_t index) override;
    IPlugin *getPlugin(const char *name) override;
    IPlugin *getPlugin(SourcePawn::IPluginContext *ctx) override;
    IPlugin *loadPlugin(const char *name,
                        char *error,
                        std::size_t size) override;

    // PluginMngr
    const auto &getPluginsList() const
    {
        return m_plugins;
    }
    void clearPlugins();
    void setPluginPrecache(bool canprecache);
    bool canPluginPrecache();
    std::shared_ptr<Plugin> loadPluginCore(std::string_view name,
                                           std::string *error);

    std::shared_ptr<Plugin> getPluginCore(std::size_t index);
    std::shared_ptr<Plugin> getPluginCore(std::string_view name);
    std::shared_ptr<Plugin> getPluginCore(SourcePawn::IPluginContext *ctx);
    std::size_t loadPlugins();

private:
    std::shared_ptr<Plugin> _loadPlugin(const fs::path &path,
                                        std::string *error);
    std::unordered_map<std::string, std::shared_ptr<Plugin>> m_plugins;

    // Allow plugins to precache
    bool m_canPluginsPrecache;
};
