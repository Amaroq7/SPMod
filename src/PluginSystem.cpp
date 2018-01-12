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
                const std::string &identity,
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

    auto forwardPtr = std::make_shared<Forward>(name,
                                                forwardParams,
                                                params,
                                                this);

    auto added = gSPGlobal->getForwardManagerCore()->addForward(forwardPtr);

    if (!added)
        return nullptr;

    return forwardPtr.get();
}

Forward *Plugin::createForward(const std::string &name,
                                const std::initializer_list<IForward::ParamType> &params) const
{
    auto paramsNum = params.size();

    if (paramsNum > SP_MAX_EXEC_PARAMS)
        return nullptr;

    std::array<IForward::ParamType, SP_MAX_EXEC_PARAMS> forwardParams;
    std::copy(params.begin(), params.end(), forwardParams.begin());

    auto forwardPtr = std::make_shared<Forward>(name,
                                                forwardParams,
                                                paramsNum,
                                                this);

    auto added = gSPGlobal->getForwardManagerCore()->addForward(forwardPtr);

    if (!added)
        return nullptr;

    return forwardPtr.get();
}

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

IPlugin *PluginMngr::loadPlugin(const char *name,
                                char *error,
                                size_t size)
{
    std::string errorMsg;

    if (!loadPluginFs(m_scriptsPath / name, errorMsg))
    {
        std::strncpy(error, errorMsg.c_str(), size);
        return nullptr;
    }

    return m_plugins.find(name)->second.get();
}

bool PluginMngr::loadPluginFs(const fs::path &path,
                                std::string &error)
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
