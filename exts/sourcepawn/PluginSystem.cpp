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

Plugin::Plugin(std::size_t id,
               std::string_view identity,
               const fs::path &path)
{
    char errorSPMsg[256];
    SourcePawn::ISourcePawnEngine2 *spAPIv2 = gSPGlobal->getSPEnvironment()->APIv2();
    SourcePawn::IPluginRuntime *plugin = spAPIv2->LoadBinaryFromFile(path.string().c_str(),
                                                                     errorSPMsg,
                                                                     sizeof(errorSPMsg));

    if (!plugin)
        throw std::runtime_error(errorSPMsg);

    uint32_t infoVarIndex;
    if (plugin->FindPubvarByName("pluginInfo", &infoVarIndex) != SP_ERROR_NONE)
        throw std::runtime_error("Can't find plugin info!");

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

    m_filename = path.filename().string();
    m_id = id;
    m_identity = identity;
    m_runtime = plugin;
    m_runtime->GetDefaultContext()->SetKey(1, const_cast<char *>(m_identity.c_str()));

    uint32_t nativesNum = plugin->GetNativesNum();
    const std::unique_ptr<PluginMngr> &plMngr = gSPGlobal->getPluginManagerCore();
    for (uint32_t index = 0; index < nativesNum; ++index)
    {
        const sp_native_t *pluginNative = m_runtime->GetNative(index);

        if (pluginNative->status == SP_NATIVE_BOUND)
            continue;

        SPVM_NATIVE_FUNC router = plMngr->findNative(pluginNative->name);
        if (!router) /* Don't mark plugin as not working yet since native can be made by other plugin */
            continue;

        plugin->UpdateNativeBinding(index, router, 0, nullptr);
    }

    // Setup maxclients num
    uint32_t maxClientsVarIndex;
    if (plugin->FindPubvarByName("maxClients", &maxClientsVarIndex) == SP_ERROR_NONE)
    {
        cell_t local_addr, *phys_addr;
        plugin->GetPubvarAddrs(maxClientsVarIndex, &local_addr, &phys_addr);
        *phys_addr = gRehldsServerStatic->GetMaxClients();
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

std::size_t Plugin::getId() const
{
    return m_id;
}

SourcePawn::IPluginRuntime *Plugin::getRuntime() const
{
    return m_runtime;
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

std::shared_ptr<Plugin> PluginMngr::getPluginCore(std::string_view name)
{
    auto result = m_plugins.find(name.data());

    return (result != m_plugins.end()) ? result->second : nullptr;
}

std::shared_ptr<Plugin> PluginMngr::getPluginCore(std::size_t index)
{
    for (const auto &entry : m_plugins)
    {
        if (entry.second->getId() == index)
            return entry.second;
    }
    return nullptr;
}

std::shared_ptr<Plugin> PluginMngr::getPluginCore(SourcePawn::IPluginContext *ctx)
{
    char *pluginIdentity;
    ctx->GetKey(1, reinterpret_cast<void **>(&pluginIdentity));

    return getPluginCore(pluginIdentity);
}

IPlugin *PluginMngr::loadPlugin(const char *name,
                                char *error,
                                std::size_t size)
{
    std::string errorMsg;
    std::shared_ptr<Plugin> plugin = loadPluginCore(name, &errorMsg);

    if (!plugin)
    {
        gSPGlobal->getUtilsCore()->strCopyCore(error, size, errorMsg);
        return nullptr;
    }

    return plugin.get();
}

std::shared_ptr<Plugin> PluginMngr::loadPluginCore(std::string_view name,
                                                    std::string *error)
{
    std::shared_ptr<Plugin> plugin = _loadPlugin(gSPGlobal->getScriptsDirCore() / name.data(), error);
    if (!plugin)
        return nullptr;

    return plugin;
}

std::shared_ptr<Plugin> PluginMngr::_loadPlugin(const fs::path &path,
                                                std::string *error)
{
    std::size_t pluginId = m_plugins.size();

    // Omit any unknown extension
    if (path.extension().string() != ".smx")
        return nullptr;

    std::string fileName = path.stem().string();
    std::shared_ptr<Plugin> plugin;
    try
    {
        plugin = std::make_shared<Plugin>(pluginId, fileName, path);
    }
    catch (const std::runtime_error &e)
    {
        *error = e.what();
        return nullptr;
    }

    if (!m_plugins.try_emplace(fileName, plugin).second)
        return nullptr;

    return plugin;
}

std::size_t PluginMngr::loadPlugins()
{
    using def = ForwardMngr::FwdDefault;

    std::error_code errCode;
    auto directoryIter = fs::directory_iterator(gSPGlobal->getScriptsDirCore(), errCode);
    auto &loggingSystem = gSPGlobal->getLoggerCore();

    if (errCode)
    {
        loggingSystem->LogMessageCore("Error while loading plugins: ", errCode.message());
        return 0;
    }

    std::string errorMsg;
    for (const auto &entry : directoryIter)
    {
        fs::path filePath = entry.path();
        if (!_loadPlugin(filePath, &errorMsg) && !errorMsg.empty())
        {
            loggingSystem->LogErrorCore(errorMsg);
            errorMsg.clear();
        }
    }

    // After first binding let plugins add their natives
    const std::unique_ptr<ForwardMngr> &fwdMngr = gSPGlobal->getForwardManagerCore();
    fwdMngr->getDefaultForward(def::PluginNatives)->execFunc(nullptr);

    // Try to bind unbound natives
    for (const auto &entry : m_plugins)
    {
        SourcePawn::IPluginRuntime *runtime = entry.second->getRuntime();
        uint32_t nativesNum = runtime->GetNativesNum();
        for (uint32_t index = 0; index < nativesNum; ++index)
        {
            const sp_native_t *pluginNative = runtime->GetNative(index);

            if (pluginNative->status == SP_NATIVE_BOUND)
                continue;

            SPVM_NATIVE_FUNC router = findNative(pluginNative->name);
            if (!router)
            {
                // TODO: At this point if native cannot be found mark plugin as not working
                break;
            }

            runtime->UpdateNativeBinding(index, router, 0, nullptr);
        }
    }

    fwdMngr->getDefaultForward(def::PluginInit)->execFunc(nullptr);
    fwdMngr->getDefaultForward(def::PluginsLoaded)->execFunc(nullptr);
    return m_plugins.size();
}

std::size_t PluginMngr::getPluginsNum() const
{
    return m_plugins.size();
}

IPlugin *PluginMngr::getPlugin(std::size_t index)
{
    return getPluginCore(index).get();
}

IPlugin *PluginMngr::getPlugin(const char *name)
{
    return getPluginCore(name).get();
}

IPlugin *PluginMngr::getPlugin(SourcePawn::IPluginContext *ctx)
{
    return getPluginCore(ctx).get();
}

void PluginMngr::clearPlugins()
{
    m_plugins.clear();
}

void PluginMngr::setPluginPrecache(bool canprecache)
{
    m_canPluginsPrecache = canprecache;
}

bool PluginMngr::canPluginPrecache()
{
    return m_canPluginsPrecache;
}

SPVM_NATIVE_FUNC PluginMngr::findNative(std::string_view name)
{
    if (auto iter = m_natives.find(name.data()); iter != m_natives.end())
        return iter->second;

    return nullptr;
}

bool PluginMngr::addNatives(const sp_nativeinfo_t *natives)
{
    while (natives->name && natives->func)
    {
        if (!_addNative(natives->name, natives->func))
            return false;

        natives++;
    }

    return true;
}

bool PluginMngr::_addNative(std::string_view name,
                            SPVM_NATIVE_FUNC func)
{
    if (!m_natives.try_emplace(name.data(), func).second)
        return false;

    return true;
}

bool PluginMngr::addFakeNative(std::string_view name,
                               SourcePawn::IPluginFunction *func)
{
    SourcePawn::ISourcePawnEngine2 *spAPIv2 = gSPGlobal->getSPEnvironment()->APIv2();
    SPVM_NATIVE_FUNC router = spAPIv2->CreateFakeNative(fakeNativeRouter, func);

    if (!_addNative(name, router))
    {
        spAPIv2->DestroyFakeNative(router);
        return false;
    }

    m_routers.push_back(router);
    return true;
}

cell_t PluginMngr::fakeNativeRouter(SourcePawn::IPluginContext *ctx,
                                    const cell_t *params,
                                    void *data)
{
    if (params[0] > SP_MAX_EXEC_PARAMS)
    {
        ctx->ReportError("Too many parameters passed to native! %d (max: %d)", params[0], SP_MAX_EXEC_PARAMS);
        return 0;
    }
    // TODO: Support later?
    if (PluginMngr::m_callerPlugin)
    {
        ctx->ReportError("Cannot call another plugin native in native callback!");
        return 0;
    }

    PluginMngr::m_callerPlugin = ctx;
    std::shared_ptr<Plugin> caller = gSPGlobal->getPluginManagerCore()->getPluginCore(ctx);

    for (std::size_t i = 0; i <= static_cast<std::size_t>(params[0]); i++)
        m_callerParams[i] = params[i];

    cell_t result = 0;

    auto *func = reinterpret_cast<SourcePawn::IPluginFunction *>(data);
    func->PushCell(caller->getId());
    func->Execute(&result);

    PluginMngr::m_callerPlugin = nullptr;

    return result;
}

void PluginMngr::clearNatives()
{
    SourcePawn::ISourcePawnEngine2 *spAPIv2 = gSPGlobal->getSPEnvironment()->APIv2();

    for (auto router : m_routers)
    {
        spAPIv2->DestroyFakeNative(router);
    }
    m_natives.clear();
}

void PluginMngr::addDefaultNatives()
{
    addNatives(gCoreNatives);
    addNatives(gCvarsNatives);
    addNatives(gForwardsNatives);
    addNatives(gStringNatives);
    addNatives(gMessageNatives);
    addNatives(gCmdsNatives);
    addNatives(gTimerNatives);
    addNatives(gFloatNatives);
}