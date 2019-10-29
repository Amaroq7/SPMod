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

#include "ExtMain.hpp"

namespace SPLuaAdapter
{
    Plugin::Plugin(std::size_t id,
                   std::string_view identity,
                   const fs::path &path,
                   const std::unique_ptr<PluginMngr> &pluginMngr)
        : m_id(id), m_identity(identity), m_filename(path.filename().string()), m_pluginMngr(pluginMngr)
    {
#if defined SP_LINUX
        int loadResult = luaL_loadfile(gLuaState, path.c_str());
#elif defined SP_WINDOWS
        int loadResult = luaL_loadfile(gLuaState, path.string().c_str());
#endif
        if (loadResult != LUA_OK)
        {
            std::stringstream errorMsg;
#if defined SP_LINUX
            errorMsg << path.filename() << ": Cannot load plugin";
#elif defined SP_WINDOWS
            errorMsg << path.filename().string() << ": Cannot load plugin";
#endif
            throw std::runtime_error(errorMsg.str());
        }
        lua_pcall(gLuaState, 0, 0, 0);

        if (lua_getglobal(gLuaState, "pluginInfo") != LUA_TTABLE)
            throw std::runtime_error(path.filename().string() + ": Can't find plugin info!");

        lua_getfield(gLuaState, -1, FIELD_NAME);
        m_name = lua_tostring(gLuaState, -1);
        lua_pop(gLuaState, 1);

        lua_getfield(gLuaState, -1, FIELD_VERSION);
        m_version = lua_tostring(gLuaState, -1);
        lua_pop(gLuaState, 1);

        lua_getfield(gLuaState, -1, FIELD_AUTHOR);
        m_author = lua_tostring(gLuaState, -1);
        lua_pop(gLuaState, 1);

        lua_getfield(gLuaState, -1, FIELD_URL);
        m_url = lua_tostring(gLuaState, -1);
        lua_pop(gLuaState, 2);

        // Setup maxclients num
        if (lua_getglobal(gLuaState, "maxClients") != LUA_TNIL)
        {
            lua_pushinteger(gLuaState, gSPGlobal->getPlayerManager()->getMaxClients());
            lua_setglobal(gLuaState, "maxClients");
            lua_pop(gLuaState, 1);
        }
    }

    const char *Plugin::getName() const
    {
        return m_name.c_str();
    }

    const char *Plugin::getVersion() const
    {
        return m_version.c_str();
    }

    const char *Plugin::getAuthor() const
    {
        return m_author.c_str();
    }

    const char *Plugin::getUrl() const
    {
        return m_url.c_str();
    }

    const char *Plugin::getIdentity() const
    {
        return m_identity.c_str();
    }

    const char *Plugin::getFilename() const
    {
        return m_filename.c_str();
    }

    SPMod::IPluginMngr *Plugin::getPluginMngr() const
    {
        return m_pluginMngr.get();
    }

    int Plugin::getProxiedParamAsInt(std::size_t index [[maybe_unused]]) const
    {
        /*try
        {
            return m_proxiedParams.at(index);
        }
        catch (const std::out_of_range &e)
        {
            return 0;
        }*/
        return 0;
    }

    int *Plugin::getProxiedParamAsIntAddr(std::size_t index [[maybe_unused]]) const
    {
        /*try
        {
            cell_t *result;
            cell_t address = m_proxiedParams.at(index);
            m_proxyContext->LocalToPhysAddr(address, &result);

            return result;
        }
        catch (const std::out_of_range &e)
        {
            return nullptr;
        }*/
        return nullptr;
    }

    float Plugin::getProxiedParamAsFloat(std::size_t index [[maybe_unused]]) const
    {
        /*try
        {
            return sp_ctof(m_proxiedParams.at(index));
        }
        catch (const std::out_of_range &e)
        {
            return 0;
        }*/
        return 0.0f;
    }

    float *Plugin::getProxiedParamAsFloatAddr(std::size_t index [[maybe_unused]]) const
    {
        /*try
        {
            cell_t *result;
            cell_t address = m_proxiedParams.at(index);
            m_proxyContext->LocalToPhysAddr(address, &result);

            return reinterpret_cast<float *>(result);
        }
        catch (const std::out_of_range &e)
        {
            return nullptr;
        }*/
        return nullptr;
    }

    char *Plugin::getProxiedParamAsString(std::size_t index [[maybe_unused]]) const
    {
        /*try
        {
            char *result;
            cell_t address = m_proxiedParams.at(index);
            m_proxyContext->LocalToString(address, &result);

            return result;
        }
        catch (const std::out_of_range &e)
        {
            return nullptr;
        }*/
        return nullptr;
    }

    void *Plugin::getProxiedParamAsArray(std::size_t index [[maybe_unused]]) const
    {
        /*try
        {
            cell_t *result;
            cell_t address = m_proxiedParams.at(index);
            m_proxyContext->LocalToPhysAddr(address, &result);

            return result;
        }
        catch (const std::out_of_range &e)
        {
            return nullptr;
        }*/
        return nullptr;
    }

    // Plugin
    std::string_view Plugin::getNameCore() const
    {
        return m_name;
    }

    std::string_view Plugin::getVersionCore() const
    {
        return m_version;
    }

    std::string_view Plugin::getAuthorCore() const
    {
        return m_author;
    }

    std::string_view Plugin::getUrlCore() const
    {
        return m_url;
    }

    std::string_view Plugin::getIdentityCore() const
    {
        return m_identity;
    }

    std::string_view Plugin::getFileNameCore() const
    {
        return m_filename;
    }

    /*void Plugin::setProxyContext(SourcePawn::IPluginContext *ctx)
    {
        m_proxyContext = ctx;
    }*/

    std::size_t Plugin::getId() const
    {
        return m_id;
    }

    /*SourcePawn::IPluginRuntime *Plugin::getRuntime() const
    {
        return m_runtime;
    }*/

    std::shared_ptr<Plugin> PluginMngr::_loadPlugin(const fs::path &path, std::string &error)
    {
        std::size_t pluginId = m_plugins.size();

        // Omit any unknown extension
        if (path.extension().string() != PluginMngr::pluginsExtension)
            return nullptr;

        std::string fileName = path.stem().string();
        std::shared_ptr<Plugin> plugin;

        if (m_plugins.find(fileName) != m_plugins.end())
            return nullptr;

        try
        {
            plugin = std::make_shared<Plugin>(pluginId, fileName, path, gInterface.getPluginMngrCore());
        }
        catch (const std::runtime_error &e)
        {
            error = e.what();
            return nullptr;
        }

        m_plugins.emplace(fileName, plugin);
        m_exportedPlugins.AddToTail(plugin.get());
        return plugin;
    }

    void PluginMngr::loadPlugins()
    {
        std::error_code errCode;
        auto directoryIter = fs::directory_iterator(gSPGlobal->getPath(SPMod::DirType::Plugins), errCode);

        if (errCode)
        {
            gLogger->logToConsole(SPMod::LogLevel::Error, "Error while loading plugins: %s", errCode.message().c_str());
            return;
        }

        std::string errorMsg;
        for (const auto &entry : directoryIter)
        {
            fs::path filePath = entry.path();
            if (!_loadPlugin(filePath, errorMsg) && !errorMsg.empty())
            {
                gLogger->logToConsole(SPMod::LogLevel::Error, "%s", errorMsg.c_str());
                errorMsg.clear();
            }
        }
    }

    SPMod::IPlugin *PluginMngr::getPlugin(const char *name)
    {
        auto iter = m_plugins.find(name);
        if (iter != m_plugins.end())
        {
            return iter->second.get();
        }
        return nullptr;
    }

    std::size_t PluginMngr::getPluginsNum() const
    {
        return m_plugins.size();
    }

    void PluginMngr::bindPluginsNatives() {}

    void PluginMngr::unloadPlugins() {}

    const char *PluginMngr::getPluginsExt() const
    {
        return ".lua";
    }

    int PluginMngr::proxyNativeCallback(const SPMod::IProxiedNative *const native [[maybe_unused]],
                                        const SPMod::IPlugin *const plugin [[maybe_unused]])
    {
        return 0;
    }

    const CUtlVector<SPMod::IPlugin *> &PluginMngr::getPluginsList() const
    {
        return m_exportedPlugins;
    }
} // namespace SPLuaAdapter
