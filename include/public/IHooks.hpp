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

#include <IHookChains.hpp>

#include <string_view>

namespace SPMod
{
    using IChangeLevelHook = IHook<void, std::string_view, std::string_view>;
    using IChangeLevelHookRegistry = IHookRegistry<void, std::string_view, std::string_view>;

    using IClientConnectHook = IHook<bool, Metamod::Engine::IEdict *, std::string_view, std::string_view, std::string &>;
    using IClientConnectHookRegistry = IHookRegistry<bool, Metamod::Engine::IEdict *, std::string_view, std::string_view, std::string &>;

    using IClientCmdHook = IHook<void, Metamod::Engine::IEdict *>;
    using IClientCmdHookRegistry = IHookRegistry<void, Metamod::Engine::IEdict *>;

    using IClientPutinServerHook = IHook<void, Metamod::Engine::IEdict *>;
    using IClientPutinServerHookRegistry = IHookRegistry<void, Metamod::Engine::IEdict *>;

    using IGameInitHook = IHook<void>;
    using IGameInitHookRegistry = IHookRegistry<void>;

    using IGameShutdownHook = IHook<void>;
    using IGameShutdownHookRegistry = IHookRegistry<void>;

    using IServerActivateHook = IHook<void, std::uint32_t, std::uint32_t>;
    using IServerActivateHookRegistry = IHookRegistry<void, std::uint32_t, std::uint32_t>;

    using IServerDeactivateHook = IHook<void>;
    using IServerDeactivateHookRegistry = IHookRegistry<void>;

    using IStartFrameHook = IHook<void>;
    using IStartFrameHookRegistry = IHookRegistry<void>;

    using IClientInfoChangedHook = IHook<void, Metamod::Engine::IEdict *, Metamod::Engine::InfoBuffer>;
    using IClientInfoChangedHookRegistry = IHookRegistry<void, Metamod::Engine::IEdict *, Metamod::Engine::InfoBuffer>;

    class IHooks
    {
    public:
        virtual ~IHooks() = default;

        virtual IChangeLevelHookRegistry *changeLevel() = 0;
        virtual IClientConnectHookRegistry *clientConnect() = 0;
        virtual IClientCmdHookRegistry *clientCmd() = 0;
        virtual IClientPutinServerHookRegistry *clientPutinServer() = 0;
        virtual IGameInitHookRegistry *gameInit() = 0;
        virtual IGameShutdownHookRegistry *gameShutdown() = 0;
        virtual IServerActivateHookRegistry *serverActivate() = 0;
        virtual IServerDeactivateHookRegistry *serverDeactivate() = 0;
        virtual IStartFrameHookRegistry *startFrame() = 0;
        virtual IClientInfoChangedHookRegistry *clientInfoChanged() = 0;
    };
}