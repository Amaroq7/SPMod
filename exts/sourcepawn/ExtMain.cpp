/*
 *  Copyright (C) 2018 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  This program is free software: you can redistribute it and/or modify
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

#include "ExtMain.hpp"
#include "SourcePawnAPI.hpp"

SPMod::ISPGlobal *gSPGlobal;
ModuleInterface gModuleInterface;

using namespace SPExt;

SPMOD_API SPMod::ExtQueryValue SPMod_Query(SPMod::ISPGlobal *spmodInstance)
{
    gSPGlobal = spmodInstance;
    gSPGlobal->registerInterface(&gModuleInterface);

    return SPMod::ExtQueryValue::SPModExt;
}

SPMOD_API bool SPMod_Init()
{
    try
    {
        gSourcePawnAPI = std::make_unique<SourcePawnAPI>(gSPGlobal->getPath(SPMod::DirType::Exts));
    }
    catch (const std::exception &e)
    {
        //TODO: Logging
        return false;
    }

    

    return true;
}

SPMOD_API void SPMod_End()
{
}
