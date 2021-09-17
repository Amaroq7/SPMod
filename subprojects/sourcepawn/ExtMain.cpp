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

#include "ExtMain.hpp"

#include "SourcePawnAPI.hpp"
#include "AdapterInterface.hpp"
#include "CmdNatives.hpp"
#include "TimerNatives.hpp"
#include "StringNatives.hpp"
#include "CoreNatives.hpp"
#include "PlayerNatives.hpp"
#include "ForwardNatives.hpp"
#include "MenuNatives.hpp"
#include "MessageNatives.hpp"
#include "PluginSystem.hpp"
#include "DebugListener.hpp"
#include "HooksNatives.hpp"

#include <ISPGlobal.hpp>
#include <metamod/IMetamod.hpp>
#include <metamod/engine/ITraceResult.hpp>
#include <metamod/IHelpers.hpp>

using namespace SPExt;

SPMod::ISPGlobal *gSPGlobal;
SPMod::ILogger *gSPLogger;
SPMod::ILoggerMngr *gSPLoggerMngr;
Metamod::IMetamod *gMetaAPI;
Metamod::Engine::ILibrary *gEngine;
Metamod::Game::ILibrary *gGame;

namespace
{
    bool checkInterfacesVersion()
    {
        bool cmdNativesInit = SPExt::initCmdNatives();
        bool timerNativesInit = SPExt::initTimerNatives();
        bool stringNativesInit = SPExt::initStringNatives();
        bool coreNativesInit = SPExt::initCoreNatives();
        bool playerNativesInit = SPExt::initPlayerNatives();
        bool forwardNativesInit = SPExt::initForwardNatives();
        bool menuNativesInit = SPExt::initMenuNatives();
        bool msgNativesInit = SPExt::initMsgNatives();

        gSPLoggerMngr = gSPGlobal->getLoggerManager();
        if (!gSPLoggerMngr->isVersionCompatible(SPMod::ILoggerMngr::VERSION))
        {
            return false;
        }

        return cmdNativesInit && timerNativesInit && stringNativesInit && coreNativesInit
               && playerNativesInit && forwardNativesInit && menuNativesInit && msgNativesInit;
    }
} // namespace

SPMOD_API SPMod::ExtQueryValue SPMod_Query(SPMod::ISPGlobal *spmodInstance, Metamod::IMetamod *metaAPI)
{
    gSPGlobal = spmodInstance;
    gAdapterInterface = std::make_shared<AdapterInterface>(gSPGlobal->getPath(SPMod::DirType::Exts));
    gMetaAPI = metaAPI;
    gEngine = metaAPI->getEngine();
    gGame = metaAPI->getGame();

    if (!checkInterfacesVersion())
    {
        return SPMod::ExtQueryValue::DontLoad;
    }

    return (gSPGlobal->registerAdapter(gAdapterInterface) ? SPMod::ExtQueryValue::SPModAdapter
                                                                : SPMod::ExtQueryValue::DontLoad);
}

SPMOD_API bool SPMod_Init()
{
    gSPLogger = gSPLoggerMngr->getLogger(gSPExtLoggerName);
    gSPLogger->setLogLevel(SPMod::LogLevel::Info);

    gSPAPI = std::make_unique<SourcePawnAPI>(gSPGlobal->getPath(SPMod::DirType::Exts));

    return true;
}

SPMOD_API void SPMod_End()
{
    uninstallHooks();
}
