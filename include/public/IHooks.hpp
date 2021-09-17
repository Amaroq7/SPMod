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

#include <anubis/engine/IEdict.hpp>
#include <anubis/engine/Common.hpp>
#include <anubis/observer_ptr.hpp>

namespace SPMod
{
    using IClientConnectHook = IHook<bool, Anubis::Engine::IEdict *, std::string_view, std::string_view, std::string &>;
    using IClientConnectHookRegistry = IHookRegistry<bool, Anubis::Engine::IEdict *, std::string_view, std::string_view, std::string &>;

    using IClientCmdHook = IHook<void, Anubis::Engine::IEdict *>;
    using IClientCmdHookRegistry = IHookRegistry<void, Anubis::Engine::IEdict *>;

    using IClientPutinServerHook = IHook<void, Anubis::Engine::IEdict *>;
    using IClientPutinServerHookRegistry = IHookRegistry<void, Anubis::Engine::IEdict *>;

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

    using IClientInfoChangedHook = IHook<void, Anubis::Engine::IEdict *, Anubis::Engine::InfoBuffer>;
    using IClientInfoChangedHookRegistry = IHookRegistry<void, Anubis::Engine::IEdict *, Anubis::Engine::InfoBuffer>;

    using IDropClientHook = IHook<void, Anubis::Engine::IEdict *, bool, std::string_view>;
    using IDropClientHookRegistry = IHookRegistry<void, Anubis::Engine::IEdict *, bool, std::string_view>;

    class IHooks
    {
    public:
        virtual ~IHooks() = default;

        virtual nstd::observer_ptr<IClientConnectHookRegistry> clientConnect() = 0;
        virtual nstd::observer_ptr<IClientCmdHookRegistry> clientCmd() = 0;
        virtual nstd::observer_ptr<IClientPutinServerHookRegistry> clientPutinServer() = 0;
        virtual nstd::observer_ptr<IGameInitHookRegistry> gameInit() = 0;
        virtual nstd::observer_ptr<IGameShutdownHookRegistry> gameShutdown() = 0;
        virtual nstd::observer_ptr<IServerActivateHookRegistry> serverActivate() = 0;
        virtual nstd::observer_ptr<IServerDeactivateHookRegistry> serverDeactivate() = 0;
        virtual nstd::observer_ptr<IStartFrameHookRegistry> startFrame() = 0;
        virtual nstd::observer_ptr<IClientInfoChangedHookRegistry> clientInfoChanged() = 0;
        virtual nstd::observer_ptr<IDropClientHookRegistry> dropClient() = 0;
    };
}