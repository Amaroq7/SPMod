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

#pragma once

#include "ext.hpp"

namespace SPExtExample
{
    class Plugin final : public SPMod::IPlugin
    {
    public:
        std::string_view getName() const override;
        std::string_view getVersion() const override;
        std::string_view getAuthor() const override;
        std::string_view getUrl() const override;
        std::string_view getIdentity() const override;
        std::string_view getFilename() const override;
        SPMod::IPluginMngr *getPluginMngr() const override;
        int getProxiedParamAsInt(std::size_t index) const override;
        int *getProxiedParamAsIntAddr(std::size_t index) const override;
        float getProxiedParamAsFloat(std::size_t index) const override;
        float *getProxiedParamAsFloatAddr(std::size_t index) const override;
        char *getProxiedParamAsString(std::size_t index) const override;
        void *getProxiedParamAsArray(std::size_t index) const override;
    };

    class PluginMngr final : public SPMod::IPluginMngr
    {
    public:
        std::size_t getPluginsNum() const override;
        Plugin *getPlugin(std::string_view name) const override;
        void loadPlugins() override;
        void bindPluginsNatives() override;
        void unloadPlugins() override;
        std::string_view getPluginsExt() const override;
        int proxyNativeCallback(SPMod::IProxiedNative *native, SPMod::IPlugin *plugin) override;
        const std::vector<SPMod::IPlugin *> &getPluginsList() const override;
    };

    extern PluginMngr gPluginMngr;
} // namespace SPExtExample