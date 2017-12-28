/*  PyMod - Python Scripting Engine for Half-Life
 *  Copyright (C) 2018  PyMod Development Team
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

#include <pymod.hpp>

class Plugin final : public IPlugin
{
public:
    Plugin(size_t id, const std::string &identity, const fs::path &path);
    ~Plugin();

    // IPlugin
    const char *getName() const { return m_name.c_str(); }
    const char *getVersion() const { return m_version.c_str(); }
    const char *getAuthor() const { return m_author.c_str(); }
    const char *getUrl() const { return m_url.c_str(); }
    const char *getIndentity() const { return m_identity.c_str(); };
    const char *getFileName() const { return m_filename.c_str(); }
    size_t getId() const { return m_id; }
    PyObject *getInternal() { return m_internal; }

    // STL
    const std::string &getNameString() const { return m_name; }
    const std::string &getVersionString() const { return m_version; }
    const std::string &getAuthorString() const { return m_author; }
    const std::string &getUrlString() const { return m_url; }
    const std::string &getIndentityString() const { return m_identity; };
    const std::string &getFileNameString() const { return m_filename; }

private:
    void GetPluginInfo(const std::string &scriptname);

    PyObject *m_internal;
    std::string m_identity;
    std::string m_filename;
    std::string m_name;
    std::string m_version;
    std::string m_author;
    std::string m_url;
    size_t m_id;
};

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
    size_t getPluginsNum() const override { return m_plugins.size(); }

    // PluginMngr
    bool loadPluginFs(const fs::path &path, std::string &error);
    const auto& getPluginsList() const { return m_plugins; }

private:
    size_t loadPlugins();
    std::unordered_map<std::string, std::shared_ptr<Plugin>> m_plugins;
    fs::path m_scriptsPath;
};
