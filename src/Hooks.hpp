/*
 *  Copyright (C) 2021 SPMod Development Team
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

#include <IHooks.hpp>
#include "HookChains.hpp"

#include <metamodcpp_sdk/engine/IEdict.hpp>
#include <metamodcpp_sdk/engine/Common.hpp>

namespace SPMod
{
    using ClientConnectHook = Hook<bool, Metamod::Engine::IEdict *, std::string_view, std::string_view, std::string &>;
    using ClientConnectHookRegistry = HookRegistry<bool, Metamod::Engine::IEdict *, std::string_view, std::string_view, std::string &>;

    using ClientCmdHook = Hook<void, Metamod::Engine::IEdict *>;
    using ClientCmdHookRegistry = HookRegistry<void, Metamod::Engine::IEdict *>;

    using ClientPutinServerHook = Hook<void, Metamod::Engine::IEdict *>;
    using ClientPutinServerHookRegistry = HookRegistry<void, Metamod::Engine::IEdict *>;

    using GameInitHook = Hook<void>;
    using GameInitHookRegistry = HookRegistry<void>;

    using GameShutdownHook = Hook<void>;
    using GameShutdownHookRegistry = HookRegistry<void>;

    using ServerActivateHook = Hook<void, std::uint32_t, std::uint32_t>;
    using ServerActivateHookRegistry = HookRegistry<void, std::uint32_t, std::uint32_t>;

    using ServerDeactivateHook = Hook<void>;
    using ServerDeactivateHookRegistry = HookRegistry<void>;

    using StartFrameHook = Hook<void>;
    using StartFrameHookRegistry = HookRegistry<void>;

    using ClientInfoChangedHook = Hook<void, Metamod::Engine::IEdict *, Metamod::Engine::InfoBuffer>;
    using ClientInfoChangedHookRegistry = HookRegistry<void, Metamod::Engine::IEdict *, Metamod::Engine::InfoBuffer>;

    using DropClientHook = Hook<void, Metamod::Engine::IEdict *, bool, std::string_view>;
    using DropClientHookRegistry = HookRegistry<void, Metamod::Engine::IEdict *, bool, std::string_view>;

    class Hooks final : public IHooks
    {
    public:
        ~Hooks() final = default;

        ClientConnectHookRegistry *clientConnect() final;
        ClientCmdHookRegistry *clientCmd() final;
        ClientPutinServerHookRegistry *clientPutinServer() final;
        GameInitHookRegistry *gameInit() final;
        GameShutdownHookRegistry *gameShutdown() final;
        ServerActivateHookRegistry *serverActivate() final;
        ServerDeactivateHookRegistry *serverDeactivate() final;
        StartFrameHookRegistry *startFrame() final;
        ClientInfoChangedHookRegistry *clientInfoChanged() final;
        DropClientHookRegistry *dropClient() final;

    private:
        ClientConnectHookRegistry m_clientConnect;
        ClientCmdHookRegistry m_clientCmd;
        ClientPutinServerHookRegistry m_clientPutinServer;
        GameInitHookRegistry m_gameInit;
        GameShutdownHookRegistry m_gameShutdown;
        ServerActivateHookRegistry m_serverActivate;
        ServerDeactivateHookRegistry m_serverDeactivate;
        StartFrameHookRegistry m_startFrame;
        ClientInfoChangedHookRegistry m_clientInfoChanged;
        DropClientHookRegistry m_dropClient;
    };
}