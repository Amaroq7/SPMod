/*
 *  Copyright (C) 2018-2021 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SPMod is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with SPMod.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

namespace SPMod
{
    class ISPGlobal;
    class ILoggerMngr;
    class ILogger;
}

namespace Metamod
{
    class IMetamod;

    namespace Engine
    {
        class ILibrary;
    }
    namespace Game
    {
        class ILibrary;
    }
}

extern SPMod::ISPGlobal *gSPGlobal;
extern SPMod::ILoggerMngr *gSPLoggerMngr;
extern SPMod::ILogger *gSPLogger;

constexpr const char *gSPExtLoggerName = "SPExt";
extern Metamod::IMetamod *gMetaAPI;
extern Metamod::Engine::ILibrary *gEngine;
extern Metamod::Game::ILibrary *gGame;