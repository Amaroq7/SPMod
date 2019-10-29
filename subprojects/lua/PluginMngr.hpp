/*
 *  Copyright (C) 2018 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  SPMod is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with SPMod.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "ExtMain.hpp"

namespace SPLuaAdapter
{
    class PluginMngr;

    class Plugin final : public SPMod::IPlugin
    {
    public:
        static constexpr const char *FIELD_NAME = "name";
        static constexpr const char *FIELD_VERSION = "version";
        static constexpr const char *FIELD_AUTHOR = "author";
        static constexpr const char *FIELD_URL = "url";

        Plugin(std::size_t id,
               std::string_view identity,
               const fs::path &path,
               const std::unique_ptr<PluginMngr> &pluginMngr);

        Plugin() = delete;
        ~Plugin() = default;

        // IPlugin
        const char *getName() const override;
        const char *getVersion() const override;
        const char *getAuthor() const override;
        const char *getUrl() const override;
        const char *getIdentity() const override;
        const char *getFilename() const override;
        SPMod::IPluginMngr *getPluginMngr() const override;
        int getProxiedParamAsInt(std::size_t index) const override;
        int *getProxiedParamAsIntAddr(std::size_t index) const override;
        float getProxiedParamAsFloat(std::size_t index) const override;
        float *getProxiedParamAsFloatAddr(std::size_t index) const override;
        char *getProxiedParamAsString(std::size_t index) const override;
        void *getProxiedParamAsArray(std::size_t index) const override;

        // Plugin
        // SourcePawn::IPluginRuntime *getRuntime() const;
        std::size_t getId() const;
        std::string_view getNameCore() const;
        std::string_view getVersionCore() const;
        std::string_view getAuthorCore() const;
        std::string_view getUrlCore() const;
        std::string_view getIdentityCore() const;
        std::string_view getFileNameCore() const;
        // void setProxyContext(SourcePawn::IPluginContext *ctx);

    private:
        std::size_t m_id;
        std::string m_identity;
        std::string m_filename;
        const std::unique_ptr<PluginMngr> &m_pluginMngr;
        std::string m_name;
        std::string m_version;
        std::string m_author;
        std::string m_url;
        // std::array<cell_t, SP_MAX_CALL_ARGUMENTS> m_proxiedParams;
        // SourcePawn::IPluginContext *m_proxyContext;
    };

    class PluginMngr final : public SPMod::IPluginMngr
    {
    public:
        PluginMngr() = default;

        constexpr static const char *pluginsExtension = ".lua";

        std::size_t getPluginsNum() const override;
        SPMod::IPlugin *getPlugin(const char *name) override;
        void loadPlugins() override;
        void bindPluginsNatives() override;
        void unloadPlugins() override;
        const char *getPluginsExt() const override;
        int proxyNativeCallback(const SPMod::IProxiedNative *const native, const SPMod::IPlugin *const plugin) override;
        const CUtlVector<SPMod::IPlugin *> &getPluginsList() const override;

    private:
        std::shared_ptr<Plugin> _loadPlugin(const fs::path &path, std::string &error);

        std::unordered_map<std::string, std::shared_ptr<Plugin>> m_plugins;
        // std::unordered_map<std::string, SPVM_NATIVE_FUNC> m_natives;

        // Routers to be freed on the map change
        // std::vector<SPVM_NATIVE_FUNC> m_routers;

        CUtlVector<SPMod::IPlugin *> m_exportedPlugins;
    };
} // namespace SPLuaAdapter