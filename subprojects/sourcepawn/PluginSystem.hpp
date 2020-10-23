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

#pragma once

#include "ExtMain.hpp"

namespace SPExt
{
    class PluginMngr;

    class Plugin final : public SPMod::IPlugin
    {
    public:
        static constexpr std::uint32_t FIELD_NAME = 0;
        static constexpr std::uint32_t FIELD_VERSION = 1;
        static constexpr std::uint32_t FIELD_AUTHOR = 2;
        static constexpr std::uint32_t FIELD_URL = 3;

        Plugin(std::size_t id, std::string_view identity, const fs::path &path, PluginMngr *pluginMngr);

        Plugin() = delete;
        ~Plugin() = default;

        // IPlugin
        std::string_view getName() const override;
        std::string_view getVersion() const override;
        std::string_view getAuthor() const override;
        std::string_view getUrl() const override;
        std::string_view getIdentity() const override;
        std::string_view getFilename() const override;
        SPMod::IPluginMngr *getPluginMngr() const override;

        // Plugin
        SourcePawn::IPluginRuntime *getRuntime() const;
        std::size_t getId() const;

    private:
        std::size_t m_id;
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
        void unloadPlugins() override;
        std::string_view getPluginsExt() const override;
        const std::vector<SPMod::IPlugin *> &getPluginsList() const override;

        // PluginMngr
        const auto &getPluginsListCore() const
        {
            return m_plugins;
        }

        bool addNatives(const sp_nativeinfo_t *natives);

        Plugin *getPlugin(std::size_t index) const;
        Plugin *getPlugin(SourcePawn::IPluginContext *ctx) const;
        Plugin *getPlugin(SPMod::IPlugin *plugin) const;

        void clearNatives();
        void addDefaultNatives();
        SPVM_NATIVE_FUNC findNative(std::string_view name);
        NativeCallback *findPluginNative(std::string_view name);

        // Proxied natives
        void addNative(SPMod::IProxiedNative *native);
        bool registerNative(std::string_view nativeName, SourcePawn::IPluginFunction *pluginFunc);

    private:
        Plugin *_loadPlugin(const fs::path &path, std::string &error);

        bool _addNative(std::string_view name, SPVM_NATIVE_FUNC func);

        std::unordered_map<std::string, std::unique_ptr<Plugin>> m_plugins;
        std::vector<SPMod::IPlugin *> m_exportedPlugins;
        std::unordered_map<std::string, SPVM_NATIVE_FUNC> m_natives;
        std::unordered_map<std::string, NativeCallback *> m_pluginNatives;
    };
} // namespace SPExt
