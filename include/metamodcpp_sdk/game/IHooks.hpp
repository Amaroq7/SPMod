/*
 *  Copyright (C) 2020 Metamod++ Development Team
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

#include "../IHookChains.hpp"

#include <string>
#include <any>

#include "../engine/Common.hpp"

namespace Metamod::Engine
{
    class IEdict;
}

namespace Metamod::Game
{
    using IGameInitHook = IHook<void>;
    using IGameInitHookRegistry = IHookRegistry<void>;

    using ISpawnHook = IHook<std::int32_t, Engine::IEdict *>;
    using ISpawnHookRegistry = IHookRegistry<std::int32_t, Engine::IEdict *>;

    using IClientConnectHook = IHook<bool, Engine::IEdict *, std::string_view, std::string_view, std::string &>;
    using IClientConnectHookRegistry = IHookRegistry<bool, Engine::IEdict *, std::string_view, std::string_view, std::string &>;

    using IClientPutinServerHook = IHook<void, Engine::IEdict *>;
    using IClientPutinServerHookRegistry = IHookRegistry<void, Engine::IEdict *>;

    using IClientCmdHook = IHook<void, Engine::IEdict *>;
    using IClientCmdHookRegistry = IHookRegistry<void, Engine::IEdict *>;

    using IClientInfoChangedHook = IHook<void, Engine::IEdict *, Engine::InfoBuffer>;
    using IClientInfoChangedHookRegistry = IHookRegistry<void, Engine::IEdict *, Engine::InfoBuffer>;

    using IServerActivateHook = IHook<void, std::uint32_t, std::uint32_t>;
    using IServerActivateHookRegistry = IHookRegistry<void, std::uint32_t, std::uint32_t>;

    using IServerDeactivateHook = IHook<void>;
    using IServerDeactivateHookRegistry = IHookRegistry<void>;

    using IStartFrameHook = IHook<void>;
    using IStartFrameHookRegistry = IHookRegistry<void>;

    using IGameShutdownHook = IHook<void>;
    using IGameShutdownHookRegistry = IHookRegistry<void>;

    class IHooks
    {
    public:
        virtual ~IHooks() = default;

        virtual IGameInitHookRegistry *gameInit() = 0;
        virtual ISpawnHookRegistry *spawn() = 0;
        virtual IClientConnectHookRegistry *clientConnect() = 0;
        virtual IClientPutinServerHookRegistry *clientPutinServer() = 0;
        virtual IClientCmdHookRegistry *clientCmd() = 0;
        virtual IClientInfoChangedHookRegistry *clientInfoChanged() = 0;
        virtual IServerActivateHookRegistry *serverActivate() = 0;
        virtual IServerDeactivateHookRegistry *serverDeactivate() = 0;
        virtual IStartFrameHookRegistry *startFrame() = 0;
        virtual IGameShutdownHookRegistry *gameShutdown() = 0;
    };
}
