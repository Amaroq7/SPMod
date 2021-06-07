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

#include "Hooks.hpp"

namespace SPMod
{
    ClientConnectHookRegistry *Hooks::clientConnect()
    {
        return &m_clientConnect;
    }

    ClientCmdHookRegistry *Hooks::clientCmd()
    {
        return &m_clientCmd;
    }

    ClientPutinServerHookRegistry *Hooks::clientPutinServer()
    {
        return &m_clientPutinServer;
    }

    GameInitHookRegistry *Hooks::gameInit()
    {
        return &m_gameInit;
    }

    GameShutdownHookRegistry *Hooks::gameShutdown()
    {
        return &m_gameShutdown;
    }

    ServerActivateHookRegistry *Hooks::serverActivate()
    {
        return &m_serverActivate;
    }

    ServerDeactivateHookRegistry *Hooks::serverDeactivate()
    {
        return &m_serverDeactivate;
    }

    StartFrameHookRegistry *Hooks::startFrame()
    {
        return &m_startFrame;
    }

    ClientInfoChangedHookRegistry *Hooks::clientInfoChanged()
    {
        return &m_clientInfoChanged;
    }
    DropClientHookRegistry *Hooks::dropClient()
    {
        return &m_dropClient;
    }
}