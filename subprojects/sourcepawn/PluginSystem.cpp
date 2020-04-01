/*
 *  Copyright (C) 2018-2020 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SPMod is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with SPMod.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "ExtMain.hpp"

namespace SPExt
{
    Plugin::Plugin(std::size_t id, std::string_view identity, const fs::path &path, PluginMngr *pluginMngr)
        : m_id(id), m_identity(identity), m_filename(path.filename().string()), m_pluginMngr(pluginMngr)
    {
        char errorSPMsg[256];
        SourcePawn::ISourcePawnEngine2 *spAPIv2 = gSPAPI->getSPEnvironment()->APIv2();
        SourcePawn::IPluginRuntime *plugin =
            spAPIv2->LoadBinaryFromFile(path.string().c_str(), errorSPMsg, sizeof(errorSPMsg));

        if (!plugin)
            throw std::runtime_error(errorSPMsg);

        std::uint32_t infoVarIndex;
        if (plugin->FindPubvarByName("pluginInfo", &infoVarIndex) != SP_ERROR_NONE)
            throw std::runtime_error(path.filename().string() + ": Can't find plugin info!");

        sp_pubvar_t *infoVar;
        plugin->GetPubvarByIndex(infoVarIndex, &infoVar);

        auto gatherInfo = [plugin, infoVar](std::uint32_t field) {
            char *infoField;
            plugin->GetDefaultContext()->LocalToString(*(infoVar->offs + field), &infoField);
            return infoField;
        };

        m_name = gatherInfo(Plugin::FIELD_NAME);
        m_version = gatherInfo(Plugin::FIELD_VERSION);
        m_author = gatherInfo(Plugin::FIELD_AUTHOR);
        m_url = gatherInfo(Plugin::FIELD_URL);

        m_runtime = plugin;
        m_runtime->GetDefaultContext()->SetKey(1, const_cast<char *>(m_identity.c_str()));

        std::uint32_t nativesNum = plugin->GetNativesNum();
        for (std::uint32_t index = 0; index < nativesNum; ++index)
        {
            const sp_native_t *pluginNative = m_runtime->GetNative(index);

            if (pluginNative->status == SP_NATIVE_BOUND)
                continue;

            SPVM_NATIVE_FUNC router = pluginMngr->findNative(pluginNative->name);
            if (!router) /* Don't mark plugin as not working yet since native can be made by other plugin */
                continue;

            plugin->UpdateNativeBinding(index, router, 0, nullptr);
        }

        // Setup maxclients num
        std::uint32_t maxClientsVarIndex;
        if (plugin->FindPubvarByName("maxClients", &maxClientsVarIndex) == SP_ERROR_NONE)
        {
            cell_t local_addr, *phys_addr;
            plugin->GetPubvarAddrs(maxClientsVarIndex, &local_addr, &phys_addr);
            *phys_addr = gSPGlobal->getPlayerManager()->getMaxClients();
        }
    }

    std::string_view Plugin::getName() const
    {
        return m_name;
    }

    std::string_view Plugin::getVersion() const
    {
        return m_version;
    }

    std::string_view Plugin::getAuthor() const
    {
        return m_author;
    }

    std::string_view Plugin::getUrl() const
    {
        return m_url;
    }

    std::string_view Plugin::getIdentity() const
    {
        return m_identity;
    }

    std::string_view Plugin::getFilename() const
    {
        return m_filename;
    }

    SPMod::IPluginMngr *Plugin::getPluginMngr() const
    {
        return m_pluginMngr;
    }

    int Plugin::getProxiedParamAsInt(std::size_t index) const
    {
        try
        {
            return m_proxiedParams.at(index);
        }
        catch (const std::out_of_range &e)
        {
            (void)e;
            return 0;
        }
    }

    int *Plugin::getProxiedParamAsIntAddr(std::size_t index) const
    {
        try
        {
            cell_t *result;
            cell_t address = m_proxiedParams.at(index);
            m_proxyContext->LocalToPhysAddr(address, &result);

            return result;
        }
        catch (const std::out_of_range &e)
        {
            (void)e;
            return nullptr;
        }
    }

    float Plugin::getProxiedParamAsFloat(std::size_t index) const
    {
        try
        {
            return sp_ctof(m_proxiedParams.at(index));
        }
        catch (const std::out_of_range &e)
        {
            (void)e;
            return 0;
        }
    }

    float *Plugin::getProxiedParamAsFloatAddr(std::size_t index) const
    {
        try
        {
            cell_t *result;
            cell_t address = m_proxiedParams.at(index);
            m_proxyContext->LocalToPhysAddr(address, &result);

            return reinterpret_cast<float *>(result);
        }
        catch (const std::out_of_range &e)
        {
            (void)e;
            return nullptr;
        }
    }

    char *Plugin::getProxiedParamAsString(std::size_t index) const
    {
        try
        {
            char *result;
            cell_t address = m_proxiedParams.at(index);
            m_proxyContext->LocalToString(address, &result);

            return result;
        }
        catch (const std::out_of_range &e)
        {
            (void)e;
            return nullptr;
        }
    }

    void *Plugin::getProxiedParamAsArray(std::size_t index) const
    {
        try
        {
            cell_t *result;
            cell_t address = m_proxiedParams.at(index);
            m_proxyContext->LocalToPhysAddr(address, &result);

            return result;
        }
        catch (const std::out_of_range &e)
        {
            (void)e;
            return nullptr;
        }
    }

    // Plugin
    void Plugin::setProxyContext(SourcePawn::IPluginContext *ctx)
    {
        m_proxyContext = ctx;
    }

    std::size_t Plugin::getId() const
    {
        return m_id;
    }

    SourcePawn::IPluginRuntime *Plugin::getRuntime() const
    {
        return m_runtime;
    }

    void Plugin::setProxyParams(const cell_t *params)
    {
        for (std::size_t i = 0; i <= static_cast<std::size_t>(params[0]); i++)
            m_proxiedParams[i] = params[i];
    }

    Plugin *PluginMngr::getPlugin(std::string_view name) const
    {
        auto result = m_plugins.find(name.data());

        return (result != m_plugins.end()) ? result->second.get() : nullptr;
    }

    Plugin *PluginMngr::getPlugin(std::size_t index) const
    {
        for (const auto &entry : m_plugins)
        {
            if (entry.second->getId() == index)
                return entry.second.get();
        }
        return nullptr;
    }

    Plugin *PluginMngr::getPlugin(SourcePawn::IPluginContext *ctx) const
    {
        char *pluginIdentity;
        ctx->GetKey(1, reinterpret_cast<void **>(&pluginIdentity));

        auto result = m_plugins.find(pluginIdentity);
        return (result != m_plugins.end()) ? result->second.get() : nullptr;
    }

    Plugin *PluginMngr::getPlugin(SPMod::IPlugin *plugin) const
    {
        for (const auto &entry : m_plugins)
        {
            if (entry.second.get() == plugin)
                return entry.second.get();
        }

        return nullptr;
    }

    Plugin *PluginMngr::_loadPlugin(const fs::path &path, std::string &error)
    {
        std::size_t pluginId = m_plugins.size();

        // Omit any unknown extension
        if (path.extension().string() != PluginMngr::pluginsExtension)
            return nullptr;

        std::string fileName = path.stem().string();
        std::unique_ptr<Plugin> plugin;

        if (m_plugins.find(fileName) != m_plugins.end())
            return nullptr;

        try
        {
            plugin = std::make_unique<Plugin>(pluginId, fileName, path, this);
        }
        catch (const std::runtime_error &e)
        {
            error = e.what();
            return nullptr;
        }

        m_exportedPlugins.emplace_back(plugin.get());
        m_plugins.emplace(fileName, std::move(plugin));

        return m_plugins.at(fileName).get();
    }

    void PluginMngr::loadPlugins()
    {
        std::error_code errCode;
        auto directoryIter = fs::directory_iterator(gSPGlobal->getPath(SPMod::DirType::Plugins), errCode);

        if (errCode)
        {
            gSPLogger->logToConsole(SPMod::LogLevel::Error, "Error while loading plugins: " + errCode.message());
            return;
        }

        addDefaultNatives();

        std::string errorMsg;
        for (const auto &entry : directoryIter)
        {
            fs::path filePath = entry.path();
            if (!_loadPlugin(filePath, errorMsg) && !errorMsg.empty())
            {
                gSPLogger->logToConsole(SPMod::LogLevel::Error, errorMsg);
                errorMsg.clear();
            }
        }
    }

    void PluginMngr::bindPluginsNatives()
    {
        for (const auto &proxiedNative : gSPNativeProxy->getProxiedNatives())
        {
            addProxiedNative(proxiedNative->getName(), proxiedNative);
        }

        for (const auto &entry : m_plugins)
        {
            SourcePawn::IPluginRuntime *runtime = entry.second->getRuntime();
            std::uint32_t nativesNum = runtime->GetNativesNum();

            for (std::uint32_t index = 0; index < nativesNum; ++index)
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
    }

    std::size_t PluginMngr::getPluginsNum() const
    {
        return m_plugins.size();
    }

    void PluginMngr::unloadPlugins()
    {
        m_plugins.clear();
    }

    std::string_view PluginMngr::getPluginsExt() const
    {
        return PluginMngr::pluginsExtension;
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
            if (!m_natives.try_emplace(natives->name, natives->func).second)
                return false;

            natives++;
        }

        return true;
    }

    bool PluginMngr::addProxiedNative(std::string_view name, SPMod::IProxiedNative *native)
    {
        SourcePawn::ISourcePawnEngine2 *spAPIv2 = gSPAPI->getSPEnvironment()->APIv2();
        SPVM_NATIVE_FUNC router = spAPIv2->CreateFakeNative(PluginMngr::proxyNativeRouter, native);

        if (!m_natives.try_emplace(name.data(), router).second)
        {
            spAPIv2->DestroyFakeNative(router);
            return false;
        }

        m_routers.push_back(router);
        return true;
    }

    cell_t PluginMngr::proxyNativeRouter(SourcePawn::IPluginContext *ctx, const cell_t *params, void *data)
    {
        if (params[0] > SP_MAX_CALL_ARGUMENTS)
        {
            ctx->ReportError("Too many parameters passed to native %d (max: %d)", params[0], SP_MAX_CALL_ARGUMENTS);
            return 0;
        }

        // TODO: Support later?
        if (PluginMngr::m_callerPlugin)
        {
            ctx->ReportError("Cannot call another plugin native in native callback");
            return 0;
        }

        Plugin *caller = gAdapterInterface->getPluginMngr()->getPlugin(ctx);
        caller->setProxyContext(ctx);
        caller->setProxyParams(params);

        cell_t result = reinterpret_cast<SPMod::IProxiedNative *>(data)->exec(caller);

        return result;
    }

    int PluginMngr::proxyNativeCallback(SPMod::IProxiedNative *native, SPMod::IPlugin *plugin)
    {
        PluginMngr::m_callerPlugin = plugin;

        cell_t result;
        auto pluginHandler = std::any_cast<SourcePawn::IPluginFunction *>(native->getData());
        pluginHandler->Execute(&result);

        PluginMngr::m_callerPlugin = nullptr;

        return result;
    }

    void PluginMngr::clearNatives()
    {
        SourcePawn::ISourcePawnEngine2 *spAPIv2 = gSPAPI->getSPEnvironment()->APIv2();

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
        addNatives(gMenuNatives);
        addNatives(gPlayerNatives);
        addNatives(gVTableNatives);
    }

    const std::vector<SPMod::IPlugin *> &PluginMngr::getPluginsList() const
    {
        return m_exportedPlugins;
    }
} // namespace SPExt