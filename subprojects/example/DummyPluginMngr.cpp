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

#include "ext.hpp"

namespace SPExtExample
{
    PluginMngr gPluginMngr;

    std::string_view Plugin::getName() const
    {
        return "Dummy plugin";
    }

    std::string_view Plugin::getVersion() const
    {
        return "1.0.0";
    }

    std::string_view Plugin::getAuthor() const
    {
        return "SPMod Development Team";
    }

    std::string_view Plugin::getUrl() const
    {
        return "https:://github.com/Amaroq7/SPMod";
    }

    std::string_view Plugin::getIdentity() const
    {
        return "dummy";
    }

    std::string_view Plugin::getFilename() const
    {
        return "dummy";
    }

    SPMod::IPluginMngr *Plugin::getPluginMngr() const
    {
        return &gPluginMngr;
    }

    int Plugin::getProxiedParamAsInt(std::size_t index [[maybe_unused]]) const
    {
        return 0;
    }

    int *Plugin::getProxiedParamAsIntAddr(std::size_t index [[maybe_unused]]) const
    {
        static int dummy = 0;
        return &dummy;
    }

    float Plugin::getProxiedParamAsFloat(std::size_t index [[maybe_unused]]) const
    {
        return 0.0f;
    }

    float *Plugin::getProxiedParamAsFloatAddr(std::size_t index [[maybe_unused]]) const
    {
        static float dummy = 0.0f;
        return &dummy;
    }

    char *Plugin::getProxiedParamAsString(std::size_t index [[maybe_unused]]) const
    {
        static char dummy[1] = {};
        return dummy;
    }

    void *Plugin::getProxiedParamAsArray(std::size_t index [[maybe_unused]]) const
    {
        static int array[1] = {};
        return array;
    }

    std::size_t PluginMngr::getPluginsNum() const
    {
        return 0;
    }

    Plugin *PluginMngr::getPlugin(std::string_view name [[maybe_unused]]) const
    {
        static Plugin dummy;
        return &dummy;
    }

    void PluginMngr::loadPlugins() {}

    void PluginMngr::bindPluginsNatives() {}

    void PluginMngr::unloadPlugins() {}

    std::string_view PluginMngr::getPluginsExt() const
    {
        return "";
    }

    int PluginMngr::proxyNativeCallback(SPMod::IProxiedNative *native [[maybe_unused]],
                                        SPMod::IPlugin *plugin [[maybe_unused]])
    {
        return 0;
    }

    const std::vector<SPMod::IPlugin *> &PluginMngr::getPluginsList() const
    {
        static std::vector<SPMod::IPlugin *> pluginsList;
        static Plugin dummyPlugin;

        if (!pluginsList.size())
        {
            pluginsList.push_back(&dummyPlugin);
        }

        return pluginsList;
    }
} // namespace SPExtExample
