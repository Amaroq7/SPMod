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

using namespace SPExt;

SPMod::ISPGlobal *gSPGlobal;
std::unique_ptr<ModuleInterface> gModuleInterface;

SPMOD_API SPMod::ExtQueryValue SPMod_Query(SPMod::ISPGlobal *spmodInstance)
{
    gSPGlobal = spmodInstance;
    gModuleInterface = std::make_unique<ModuleInterface>(gSPGlobal->getPath(DirType::Exts));

    return (gSPGlobal->registerInterface(gModuleInterface.get()) ? SPMod::ExtQueryValue::SPModExt : SPMod::ExtQueryValue::DontLoad);
}

SPMOD_API bool SPMod_Init()
{
    gSPGlobal->getForwardManager()->addForwardListener(Listener::Forward);
    return true;
}

SPMOD_API void SPMod_End()
{
}
