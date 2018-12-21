/*
 *  Copyright (C) 2018 SPMod Development Team
 *
 *  This file is part of SPMod.
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

#include "ExtMain.hpp"

namespace SPExt
{
    class Plugin final : public SPMod::IPlugin
    {
    public:
        static constexpr std::uint32_t FIELD_NAME = 0;
        static constexpr std::uint32_t FIELD_VERSION = 1;
        static constexpr std::uint32_t FIELD_AUTHOR = 2;
        static constexpr std::uint32_t FIELD_URL = 3;

        Plugin(std::size_t id,
            std::string_view identity,
            const fs::path &path,
            std::shared_ptr<PluginMngr> pluginMngr);

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
        const char *getProxiedParamAsString(std::size_t index) const override;
        void *getProxiedParamAsArray(std::size_t index) const override;

        // Plugin
        SourcePawn::IPluginRuntime *getRuntime() const;
        std::size_t getId() const;
        std::string_view getNameCore() const;
        std::string_view getVersionCore() const;
        std::string_view getAuthorCore() const;
        std::string_view getUrlCore() const;
        std::string_view getIdentityCore() const;
        std::string_view getFileNameCore() const;

    private:
        std::weak_ptr<PluginMngr> m_pluginMngr;
        SourcePawn::IPluginRuntime *m_runtime;
        std::string m_identity;
        std::string m_filename;
        std::string m_name;
        std::string m_version;
        std::string m_author;
        std::string m_url;
        std::size_t m_id;
        std::array<cell_t, SP_MAX_CALL_ARGUMENTS> m_proxiedParams;
    };

    class PluginMngr final : public SPMod::IPluginMngr
    {
    public:
        constexpr static const char *PluginsExtension = ".smx";

        PluginMngr() = default;
        ~PluginMngr() = default;

        // IPluginMngr
        std::size_t getPluginsNum() const override;
        SPMod::IPlugin *getPlugin(const char *name) override;
        void loadPlugins() override;
        void bindPluginsNatives() override;
        void unloadPlugins() override;
        const char *getPluginsExt() override;

        // PluginMngr
        const auto &getPluginsList() const
        {
            return m_plugins;
        }

        bool addNatives(const sp_nativeinfo_t *natives);

        std::shared_ptr<Plugin> getPluginCore(std::size_t index) const;
        std::shared_ptr<Plugin> getPluginCore(std::string_view name) const;
        std::shared_ptr<Plugin> getPluginCore(SourcePawn::IPluginContext *ctx) const;
        std::shared_ptr<Plugin> getPluginCore(SPMod::IPlugin *plugin) const;

        void clearNatives();
        void addDefaultNatives();
        SPVM_NATIVE_FUNC findNative(std::string_view name);

        // Proxied natives
        bool addProxiedNative(std::string_view name,
                              SPMod::IProxiedNative *native);

        static cell_t proxyNativeRouter(SourcePawn::IPluginContext *ctx,
                                        const cell_t *params,
                                        void *data);

        static inline SPMod::IPlugin *m_callerPlugin;

    private:
        std::shared_ptr<Plugin> _loadPlugin(const fs::path &path,
                                            std::string &error);

        bool _addNative(std::string_view name,
                        SPVM_NATIVE_FUNC func);

        std::unordered_map<std::string, std::shared_ptr<Plugin>> m_plugins;
        std::unordered_map<std::string, SPVM_NATIVE_FUNC> m_natives;

        // Routers to be freed on the map change
        std::vector<SPVM_NATIVE_FUNC> m_routers;
    };
}
