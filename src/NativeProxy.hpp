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

#pragma once

#include "spmod.hpp"

class ProxiedNative final : public IProxiedNative
{
public:
    ProxiedNative() = delete;
    ProxiedNative(const ProxiedNative &other) = delete;
    ProxiedNative(ProxiedNative &&other) = default;
    ~ProxiedNative() = default;

    ProxiedNative(std::string_view name, std::any data, IPlugin *plugin);

    std::string_view getName() const override;
    std::any getData() const override;
    IPlugin *getPlugin() const override;
    std::int32_t exec(IPlugin *plugin) override;

private:
    IPlugin *m_plugin;
    std::string m_name;
    std::any m_data;
};

class NativeProxy final : public INativeProxy
{
public:
    NativeProxy() = default;
    NativeProxy(const NativeProxy &other) = delete;
    NativeProxy(NativeProxy &&other) = default;
    ~NativeProxy() = default;

    bool registerNative(std::string_view name, std::any data, IPlugin *plugin) override;

    const std::unordered_map<std::string, std::unique_ptr<ProxiedNative>> &getProxiedNatives() const;
    void clearNatives();

protected:
    std::vector<IProxiedNative *> getProxiedNativesImpl() const override;

private:
    std::unordered_map<std::string, std::unique_ptr<ProxiedNative>> m_proxiedNatives;
};