/*
 *  Copyright (C) 2018-2020 SPMod Development Team
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

using namespace SPExt;

SPMod::ISPGlobal *gSPGlobal;
std::unique_ptr<AdapterInterface> gAdapterInterface;
SPMod::ILogger *gSPLogger;

SPMOD_API SPMod::ExtQueryValue SPMod_Query(SPMod::ISPGlobal *spmodInstance)
{
    gSPGlobal = spmodInstance;
    gAdapterInterface = std::make_unique<AdapterInterface>(gSPGlobal->getPath(SPMod::DirType::Exts));

    return (gSPGlobal->registerAdapter(gAdapterInterface.get()) ? SPMod::ExtQueryValue::SPModAdapter
                                                                : SPMod::ExtQueryValue::DontLoad);
}

SPMOD_API bool SPMod_Init()
{
    gSPLogger = gSPGlobal->getLoggerManager()->getLogger(gSPExtLoggerName);
    gSPLogger->setLogLevel(SPMod::LogLevel::Info);

    gSPAPI = std::make_unique<SourcePawnAPI>(gSPGlobal->getPath(SPMod::DirType::Exts));

    gSPGlobal->getForwardManager()->addForwardListener(Listener::Forward);
    return true;
}

SPMOD_API void SPMod_End() {}
