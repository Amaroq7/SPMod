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

#include "spmod.hpp"

class ProxiedNative final : public IProxiedNative
{
public:
    ProxiedNative() = delete;
    ProxiedNative(const ProxiedNative &other) = delete;
    ProxiedNative(ProxiedNative &&other) = default;
    ~ProxiedNative() = default;

    ProxiedNative(std::size_t id,
                  std::string_view name,
                  void *data,
                  const IPlugin *plugin);

    const char *getName() const override;
    void *getData() const override;
    const IPlugin *getPlugin() const override;

    std::size_t getId() const;

private:
    std::size_t m_id;
    const IPlugin *m_plugin;
    std::string m_name;
    void *m_data;
};

class NativeProxy final : public INativeProxy
{
public:
    NativeProxy() = delete;
    NativeProxy(const NativeProxy &other) = delete;
    NativeProxy(NativeProxy &&other) = default;
    ~NativeProxy() = default;

    bool registerNative(const char *name,
                        void *data,
                        const IPlugin *plugin) override;

    int nativeExecNotify(const IProxiedNative *native,
                         const IPlugin *const plugin) const override;

    const IProxiedNative *getProxiedNative(std::size_t id) const override;
    std::size_t getNativesNum() const override;

private:
    std::size_t m_nativeId;
    std::unordered_map<std::string, std::shared_ptr<ProxiedNative>> m_proxiedNatives;
};