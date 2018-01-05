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

#include <pluginMngr.hpp>

IPlugin *PluginMngr::getPlugin(size_t index)
{
    for (const auto &entry : m_plugins)
    {
        if (entry.second->getId() == index)
            return entry.second.get();
    }
    return nullptr;
}

IPlugin *PluginMngr::getPlugin(const char *name)
{
    auto result = m_plugins.find(name);

    return (result != m_plugins.end()) ? result->second.get() : nullptr;
}

void PluginMngr::unloadPlugin(const char *name)
{
    m_plugins.erase(name);
}

PluginMngr::PluginMngr(const fs::path &pathToScripts)
{
    m_scriptsPath = std::move(pathToScripts);
}

IPlugin *PluginMngr::loadPlugin(const char *name, char *error, size_t size)
{
    std::string errorMsg;

    if (!loadPluginFs(m_scriptsPath / name, errorMsg))
    {
        std::strncpy(error, errorMsg.c_str(), size);
        return nullptr;
    }

    return m_plugins.find(name)->second.get();
}

bool PluginMngr::loadPluginFs(const fs::path &path, std::string &error)
{
    auto pluginId = m_plugins.size();
    auto retResult = false;

    if (path.extension().string() != ".smx")
    {
        std::stringstream msg;
        msg << "[SPMod] Unrecognized file format: " << path << '\n';
        error = msg.str();

        return retResult;
    }

    auto fileName = path.stem().string();
    try
    {
        auto result = m_plugins.try_emplace(fileName, std::make_shared<Plugin>(pluginId, fileName, path));
        retResult = result.second;
    }
    catch (const std::exception &e)
    {
        std::stringstream msg;
        msg << "[SPMod] " << e.what() << '\n';
        error = msg.str();
    }

    return retResult;
}

size_t PluginMngr::loadPlugins()
{
    std::error_code errCode;
    auto directoryIter = fs::directory_iterator(m_scriptsPath, errCode);

    if (errCode)
    {
        std::stringstream msg;
        msg << "[SPMod] Error while loading plugins: " << errCode.message() << '\n';
        SERVER_PRINT(msg.str().c_str());

        return 0;
    }

    std::string errorMsg;
    for (const auto &entry : directoryIter)
    {
        auto filePath = entry.path();
        if (!loadPluginFs(filePath, errorMsg))
        {
            SERVER_PRINT(errorMsg.c_str());
            errorMsg.clear();
        }
    }
    return m_plugins.size();
}

void PluginMngr::detachPlugins()
{
    m_plugins.clear();
}
