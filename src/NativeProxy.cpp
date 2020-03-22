/*
 *  Copyright (C) 2018-2020 SPMod Development Team
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

#include "spmod.hpp"

ProxiedNative::ProxiedNative(std::string_view name, std::any data, IPlugin *plugin)
    : m_plugin(plugin), m_name(name), m_data(data)
{
}

std::string_view ProxiedNative::getName() const
{
    return m_name;
}

std::any ProxiedNative::getData() const
{
    return m_data;
}

IPlugin *ProxiedNative::getPlugin() const
{
    return m_plugin;
}

std::int32_t ProxiedNative::exec(IPlugin *plugin)
{
    return getPlugin()->getPluginMngr()->proxyNativeCallback(this, plugin);
}

bool NativeProxy::registerNative(std::string_view name, std::any data, IPlugin *plugin)
{
    return m_proxiedNatives.try_emplace(name.data(), std::make_unique<ProxiedNative>(name, data, plugin)).second;
}

const std::unordered_map<std::string, std::unique_ptr<ProxiedNative>> &NativeProxy::getProxiedNatives() const
{
    return m_proxiedNatives;
}

void NativeProxy::clearNatives()
{
    m_proxiedNatives.clear();
}

std::vector<IProxiedNative *> NativeProxy::getProxiedNativesImpl() const
{
    std::vector<IProxiedNative *> proxiedNatives;
    for (const auto &[name, native] : getProxiedNatives())
    {
        proxiedNatives.emplace_back(native.get());
    }

    return proxiedNatives;
}
