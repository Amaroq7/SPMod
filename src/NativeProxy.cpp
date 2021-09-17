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

#include "NativeProxy.hpp"

bool NativeProxy::registerNative(std::weak_ptr<IProxiedNative> native)
{
    if (native.expired())
    {
        return false;
    }

    return m_proxiedNatives.try_emplace(native.lock()->getName().data(), native).second;
}

const std::unordered_map<std::string, std::weak_ptr<IProxiedNative>> &NativeProxy::getProxiedNatives() const
{
    return m_proxiedNatives;
}
