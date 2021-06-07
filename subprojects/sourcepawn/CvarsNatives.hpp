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

#include <IForwardSystem.hpp>

#include "TypeHandler.hpp"

typedef struct sp_nativeinfo_s sp_nativeinfo_t;

namespace SPExt
{
    bool initCvarsNatives();
}

namespace Metamod
{
    class IHookInfo;
    namespace Engine
    {
        class ICvar;
    }
}

namespace SourcePawn
{
    class IPluginFunction;
}

extern TypeHandler<Metamod::Engine::ICvar> gCvarsHandlers;
extern TypeHandler<Metamod::IHookInfo> gCvarsCallbackHandlers;
extern std::unordered_multimap<Metamod::Engine::ICvar *, Metamod::IHookInfo *> gCvarPluginsCallbacks;

extern sp_nativeinfo_t gCvarsNatives[];