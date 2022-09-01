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

#include "ExtMain.hpp"
#include "TypeHandler.hpp"

#include <IAccessSystem.hpp>

typedef struct sp_nativeinfo_s sp_nativeinfo_t;

namespace SPExt
{
    bool initAccessNatives();
}

extern std::weak_ptr<SPMod::IGroupMngr> gAccessGroupMngr;
extern TypeHandler<SPMod::IAccessGroup> gAccessGroupHandlers;

extern sp_nativeinfo_t gAccessNatives[];