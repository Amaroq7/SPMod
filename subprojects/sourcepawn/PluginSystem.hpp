/*
 *  Copyright (C) 2018-2021 SPMod Development Team
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

#pragma once

#include <IPluginSystem.hpp>

#include "SourcePawnAPI.hpp"

namespace SPMod
{
    class IProxiedNative;
}

namespace SourcePawn
{
    class IPluginRuntime;
    class IPluginContext;
}

namespace SPExt
{
    class PluginMngr;
    class NativeCallback;

    class Plugin final : public ::SPMod::IPlugin
    {
    public:
        static constexpr std::uint32_t FIELD_NAME = 0;
        static constexpr std::uint32_t FIELD_VERSION = 1;
        static constexpr std::uint32_t FIELD_AUTHOR = 2;
        static constexpr std::uint32_t FIELD_URL = 3;

        static constexpr std::uint32_t KEY_IDENTITY = 1;
        static constexpr std::uint32_t KEY_PLUGIN = 2;

        Plugin(std::string_view identity, const fs::path &path, PluginMngr *pluginMngr);

        Plugin() = delete;
        ~Plugin() final = default;

        // IPlugin
        std::string_view getName() const final;
        std::string_view getVersion() const final;
        std::string_view getAuthor() const final;
        std::string_view getUrl() const final;
        std::string_view getIdentity() const final;
        std::string_view getFilename() const final;
        SPMod::IPluginMngr *getPluginMngr() const final;

        // Plugin
        SourcePawn::IPluginRuntime *getRuntime() const;

    private:
        std::string m_identity;
        std::string m_filename;
        PluginMngr *m_pluginMngr;
        SourcePawn::IPluginRuntime *m_runtime;
        std::string m_name;
        std::string m_version;
        std::string m_author;
        std::string m_url;
    };

    class PluginMngr final : public SPMod::IPluginMngr
    {
    public:
        constexpr static const char *pluginsExtension = ".smx";

        PluginMngr() = default;
        ~PluginMngr() = default;

        // IPluginMngr
        std::size_t getPluginsNum() const override;
        Plugin *getPlugin(std::string_view name) const override;
        void loadPlugins() override;
        void bindPluginsNatives() override;
        std::string_view getPluginsExt() const override;
        const std::vector<SPMod::IPlugin *> &getPluginsList() const override;

        // PluginMngr
        const auto &getPluginsListImpl() const
        {
            return m_plugins;
        }

        bool addNatives(const sp_nativeinfo_t *natives);

        Plugin *getPlugin(std::size_t index) const;
        Plugin *getPlugin(SourcePawn::IPluginContext *ctx) const;
        Plugin *getPlugin(SPMod::IPlugin *plugin) const;

        void addDefaultNatives();
        SPVM_NATIVE_FUNC findNative(std::string_view name);
        NativeCallback *findPluginNative(std::string_view name);

        // Proxied natives
        void addNative(SPMod::IProxiedNative *native);
        bool registerNative(std::string_view nativeName, SourcePawn::IPluginFunction *pluginFunc);

    private:
        Plugin *_loadPlugin(const fs::path &path, std::string &error);

        std::unordered_map<std::string, std::unique_ptr<Plugin>> m_plugins;
        std::vector<SPMod::IPlugin *> m_exportedPlugins;
        std::unordered_map<std::string, SPVM_NATIVE_FUNC> m_natives;
        std::unordered_map<std::string, NativeCallback *> m_pluginNatives;
    };
} // namespace SPExt
