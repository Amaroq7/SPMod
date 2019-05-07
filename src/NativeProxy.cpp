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

#include "spmod.hpp"

ProxiedNative::ProxiedNative(std::size_t id,
                             std::string_view name,
                             void *data,
                             const IPlugin *plugin) : m_id(id),
                                                      m_plugin(plugin),
                                                      m_name(name),
                                                      m_data(data)
{
}

const char *ProxiedNative::getName() const
{
    return m_name.c_str();
}

void *ProxiedNative::getData() const
{
    return m_data;
}

const IPlugin *ProxiedNative::getPlugin() const
{
    return m_plugin;
}

std::size_t ProxiedNative::getId() const
{
    return m_id;
}

bool NativeProxy::registerNative(const char *name, void *data, const IPlugin *plugin)
{
    return m_proxiedNatives.try_emplace(name, std::make_shared<ProxiedNative>(m_nativeId++, name, data, plugin)).second;
}

int NativeProxy::nativeExecNotify(const IProxiedNative *const native, const IPlugin *const plugin) const
{
    return native->getPlugin()->getPluginMngr()->proxyNativeCallback(native, plugin);
}

const IProxiedNative *NativeProxy::getProxiedNative(std::size_t id) const
{
    for (const auto &native : m_proxiedNatives)
    {
        if (native.second->getId() == id)
            return native.second.get();
    }

    return nullptr;
}

std::size_t NativeProxy::getNativesNum() const
{
    return m_proxiedNatives.size();
}
