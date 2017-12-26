/*  PyMod - Python Half-Life scripting engine
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

class Plugin //final : public IPlugin
{
public:
    Plugin(size_t id, const fs::path &path);
    ~Plugin();

    const char *getName() const { return m_name.c_str(); }
    const char *getVersion() const { return m_version.c_str(); }
    const char *getAuthor() const { return m_author.c_str(); }
    const char *getUrl() const { return m_url.c_str(); }
    const char *getFileName() { return m_filename.c_str(); }
    size_t getId() const { return m_id; }
    PyObject *getInternal() { return m_internal; }

private:
    void GetPluginInfo(const std::string &scriptname);

    PyObject *m_internal;
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

    Plugin *loadPlugin(const char *name, char *error, size_t size);
    void unloadPlugin(size_t index);

    //Plugin *findPlugin(size_t index);
    //Plugin *findPlugin(const char *name);

    size_t getPluginsNum() const { return m_plugins.size(); }
    const auto& getPluginsList() const { return m_plugins; }
private:
    size_t loadPlugins();
    std::map<size_t, std::shared_ptr<Plugin>> m_plugins;
    fs::path m_scriptsPath;
};
