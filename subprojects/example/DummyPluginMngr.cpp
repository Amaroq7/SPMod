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

#include "ext.hpp"

namespace SPExtExample
{
    PluginMngr gPluginMngr;

    const char *Plugin::getName() const
    {
        return "Dummy plugin";
    }

    const char *Plugin::getVersion() const
    {
        return "1.0.0";
    }

    const char *Plugin::getAuthor() const
    {
        return "SPMod Development Team";
    }

    const char *Plugin::getUrl() const
    {
        return "https:://github.com/Amaroq7/SPMod";
    }

    const char *Plugin::getIdentity() const
    {
        return "dummy";
    }

    const char *Plugin::getFilename() const
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

    SPMod::IPlugin *PluginMngr::getPlugin(const char *name [[maybe_unused]])
    {
        static Plugin dummy;
        return &dummy;
    }

    void PluginMngr::loadPlugins()
    {
    }

    void PluginMngr::bindPluginsNatives()
    {
    }

    void PluginMngr::unloadPlugins()
    {
    }

    const char *PluginMngr::getPluginsExt() const
    {
        return "";
    }

    int PluginMngr::proxyNativeCallback(const SPMod::IProxiedNative *const native [[maybe_unused]],
                                        const SPMod::IPlugin *const plugin [[maybe_unused]])
    {
        return 0;
    }

    const CUtlVector<SPMod::IPlugin *> &PluginMngr::getPluginsList() const
    {
        static CUtlVector<SPMod::IPlugin *> pluginsList;
        static Plugin dummyPlugin;

        if (!pluginsList.Count())
        {
            pluginsList.AddToTail(&dummyPlugin);
        }

        return pluginsList;
    }
}
