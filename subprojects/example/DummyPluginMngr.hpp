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

#include "ext.hpp"

namespace SPExtExample
{
    class Plugin final : public SPMod::IPlugin
    {
    public:

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
    };

    class PluginMngr final : public SPMod::IPluginMngr
    {
    public:
        std::size_t getPluginsNum() const override;
        SPMod::IPlugin *getPlugin(const char *name) override;
        void loadPlugins() override;
        void bindPluginsNatives() override;
        void unloadPlugins() override;
        const char *getPluginsExt() const override;
        int proxyNativeCallback(const SPMod::IProxiedNative *const native, const SPMod::IPlugin *const plugin) override;
    };

    extern PluginMngr gPluginMngr;
}