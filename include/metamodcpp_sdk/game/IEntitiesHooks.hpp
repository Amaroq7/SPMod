/*
 *  Copyright (C) 2020-2021 Metamod++ Development Team
 *
 *  This file is part of Metamod++.
 *
 *  Metamod++ is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  Metamod++ is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with Metamod++.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "ILibrary.hpp"
#include "../IHookChains.hpp"
#include <string>

namespace Metamod::Engine
{
    class IEdict;
    class IEntVars;
    class ITraceResult;
}

namespace Metamod::Game
{
    class IBaseEntity;
    class IBasePlayer;

    using IBasePlayerSpawnHook = IClassHook<void, IBasePlayer *>;
    using IBasePlayerSpawnHookRegistry = IClassHookRegistry<void, IBasePlayer *>;

    using IBasePlayerTakeDamageHook = IClassHook<bool, IBasePlayer *, Engine::IEntVars *, Engine::IEntVars *, float, std::int32_t>;
    using IBasePlayerTakeDamageHookRegistry = IClassHookRegistry<bool, IBasePlayer *, Engine::IEntVars *, Engine::IEntVars *, float, std::int32_t>;

    using IBasePlayerTraceAttackHook = IClassHook<void, IBasePlayer *, Engine::IEntVars *, float, float *, Engine::ITraceResult *, std::int32_t>;
    using IBasePlayerTraceAttackHookRegistry = IClassHookRegistry<void, IBasePlayer *, Engine::IEntVars *, float, float *, Engine::ITraceResult *, std::int32_t>;

    using IBasePlayerKilledHook = IClassHook<void, IBasePlayer *, Engine::IEntVars *, GibType>;
    using IBasePlayerKilledHookRegistry = IClassHookRegistry<void, IBasePlayer *, Engine::IEntVars *, GibType>;

    class IBasePlayerHooks
    {
    public:
        virtual ~IBasePlayerHooks() = default;

        virtual IBasePlayerSpawnHookRegistry *spawn() = 0;
        virtual IBasePlayerTakeDamageHookRegistry *takeDamage() = 0;
        virtual IBasePlayerTraceAttackHookRegistry *traceAttack() = 0;
        virtual IBasePlayerKilledHookRegistry *killed() = 0;
    };
}
