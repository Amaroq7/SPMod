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

#include "PluginSystem.hpp"

Plugin::Plugin(size_t id,
                std::string_view identity,
                const fs::path &path)
{
    char errorSPMsg[256];
    std::stringstream errorMsg;
    auto *spAPIv2 = gSPGlobal->getSPEnvironment()->APIv2();
    auto *plugin = spAPIv2->LoadBinaryFromFile(path.string().c_str(), errorSPMsg, sizeof(errorSPMsg));

    if (!plugin)
    {
        errorMsg << "Can't load " << path.filename() << "! (" << errorSPMsg << ")";
        throw std::runtime_error(errorMsg.str());
    }

    uint32_t infoVarIndex;
    if (plugin->FindPubvarByName("pluginInfo", &infoVarIndex) != SP_ERROR_NONE)
    {
        errorMsg << "Can't find plugin info! (" << path.filename() << ")";
        throw std::runtime_error(errorMsg.str());
    }

    sp_pubvar_t *infoVar;
    plugin->GetPubvarByIndex(infoVarIndex, &infoVar);

    auto gatherInfo = [plugin, infoVar](uint32_t field)
    {
        char *infoField;
        plugin->GetDefaultContext()->LocalToString(*(infoVar->offs + field), &infoField);
        return infoField;
    };

    m_name = gatherInfo(Plugin::FIELD_NAME);
    m_version = gatherInfo(Plugin::FIELD_VERSION);
    m_author = gatherInfo(Plugin::FIELD_AUTHOR);
    m_url = gatherInfo(Plugin::FIELD_URL);

    m_filename = path.filename();
    m_id = id;
    m_identity = identity;
    m_runtime = plugin;

    auto nativesNum = plugin->GetNativesNum();
    for (uint32_t index = 0; index < nativesNum; ++index)
    {
        const sp_native_t *native = m_runtime->GetNative(index);

        if (native->status == SP_NATIVE_BOUND)
            continue;

        for (const auto &entry : gSPGlobal->getModulesList())
        {
            for (auto nativePos = 0U; nativePos <= entry.second.num; ++nativePos)
            {
                auto nativeDef = entry.second.natives + nativePos;

                if (std::strcmp(native->name, nativeDef->name))
                    continue;

                plugin->UpdateNativeBinding(index, nativeDef->func, 0, nullptr);
                break;
            }
        }
    }

    cell_t result = 0;
    auto *initFunction = plugin->GetFunctionByName("pluginInit");

    if (initFunction)
        initFunction->Execute(&result);
}

Plugin::~Plugin()
{
    cell_t result;
    auto *endFunction = m_runtime->GetFunctionByName("pluginEnd");

    if (endFunction)
        endFunction->Execute(&result);
}

IForward *Plugin::createForward(const char *name,
                                size_t params,
                                ...) const
{
    if (params > SP_MAX_EXEC_PARAMS)
        return nullptr;

    std::array<Forward::ParamType, SP_MAX_EXEC_PARAMS> forwardParams;

    // Get passed params types
    va_list paramsList;
    va_start(paramsList, params);

    for (auto i = 0U; i < params; ++i)
        forwardParams.at(i) = static_cast<Forward::ParamType>(va_arg(paramsList, int));

    va_end(paramsList);

    return _createForward(name, forwardParams, params).get();
}

std::shared_ptr<Forward> Plugin::createForwardCore(std::string_view name,
                                                    std::initializer_list<IForward::ParamType> params) const
{
    auto paramsNum = params.size();

    if (paramsNum > SP_MAX_EXEC_PARAMS)
        return nullptr;

    std::array<IForward::ParamType, SP_MAX_EXEC_PARAMS> forwardParams;
    std::copy(params.begin(), params.end(), forwardParams.begin());

    return _createForward(name, forwardParams, paramsNum);
}

std::shared_ptr<Forward> Plugin::_createForward(std::string_view name,
                                                std::array<IForward::ParamType, SP_MAX_EXEC_PARAMS> paramlist,
                                                size_t paramsnum) const
{
    auto forwardPtr = std::make_shared<Forward>(name, paramlist, paramsnum, this);

    if (!gSPGlobal->getForwardManagerCore()->addForward(forwardPtr))
        return nullptr;

    return forwardPtr;
}

IPlugin *PluginMngr::getPlugin(const char *name)
{
    return getPluginCore(name).get();
}

IPlugin *PluginMngr::getPlugin(size_t index)
{
    return getPluginCore(index).get();
}

std::shared_ptr<Plugin> PluginMngr::getPluginCore(std::string_view name)
{
    auto result = m_plugins.find(name.data());

    return (result != m_plugins.end()) ? result->second : nullptr;
}

std::shared_ptr<Plugin> PluginMngr::getPluginCore(size_t index)
{
    for (const auto &entry : m_plugins)
    {
        if (entry.second->getId() == index)
            return entry.second;
    }
    return nullptr;
}

IPlugin *PluginMngr::loadPlugin(const char *name,
                                char *error,
                                size_t size)
{
    std::string errorMsg;
    auto plugin = loadPluginCore(name, &errorMsg);

    if (!plugin)
    {
        std::strncpy(error, errorMsg.c_str(), size);
        return nullptr;
    }

    return plugin.get();
}

std::shared_ptr<Plugin> PluginMngr::loadPluginCore(std::string_view name,
                                                    std::string *error)
{
    auto plugin = _loadPlugin(m_scriptsPath / name, error);
    if (!plugin)
        return nullptr;

    return plugin;
}

std::shared_ptr<Plugin> PluginMngr::_loadPlugin(const fs::path &path,
                                                std::string *error)
{
    auto pluginId = m_plugins.size();

    if (path.extension().string() != ".smx")
    {
        std::stringstream msg;
        msg << "[SPMod] Unrecognized file format: " << path << '\n';
        *error = msg.str();

        return nullptr;
    }

    auto fileName = path.stem().string();
    std::shared_ptr<Plugin> plugin;
    try
    {
        plugin = std::make_shared<Plugin>(pluginId, fileName, path);

        if (!m_plugins.try_emplace(fileName, plugin).second)
            return nullptr;
    }
    catch (const std::exception &e)
    {
        std::stringstream msg;
        msg << "[SPMod] " << e.what() << '\n';
        *error = msg.str();

        return nullptr;
    }

    return plugin;
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
        if (!_loadPlugin(filePath, &errorMsg))
        {
            SERVER_PRINT(errorMsg.c_str());
            errorMsg.clear();
        }
    }
    return m_plugins.size();
}
