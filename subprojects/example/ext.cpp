/*
 *  Copyright (C) 2018 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "ext.hpp"

SPMod::ISPGlobal *gSPGlobal;
SPMod::ILogger *gLogger;
SPExtExample::Interface gInterface;

SPMOD_API SPMod::ExtQueryValue SPMod_Query(SPMod::ISPGlobal *spmodInstance)
{
    gSPGlobal = spmodInstance;
    gSPGlobal->registerAdapter(&gInterface);

    return SPMod::ExtQueryValue::SPModAdapter;
}

SPMOD_API bool SPMod_Init()
{
    gLogger = gSPGlobal->getLoggerManager()->getLogger("SPExample");
    gLogger->setLogLevel(SPMod::LogLevel::Info);
    gLogger->logToBoth(SPMod::LogLevel::Info, "Logger initialized");

    return true;
}

SPMOD_API void SPMod_End() {}
