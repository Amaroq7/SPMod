/*
 *  Copyright (C) 2020-2021 Metamod++ Development Team
 *
 *  This file is part of Metamod++.
 *
 *  Metamod++ is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  Metamod++ is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with Metamod++.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

namespace Metamod::Engine
{
    class IEdict;
    class IEntVars;
    class ITraceResult;
}

#include <string_view>

namespace Metamod::Game
{
    class IBaseEntity 
    {
        public:
            virtual ~IBaseEntity() = default;
            virtual Engine::IEdict *edict() const = 0;
            virtual void remove() = 0;
            virtual bool isAlive() const = 0;
            virtual std::string_view getTeam() const = 0;
            virtual int takeHealth(float flHealth, int bitsDamageType) const = 0;
            virtual bool takeDamage(Engine::IEntVars *pevInflictor, Engine::IEntVars *pevAttacker, float flDamage, std::int32_t bitsDamageType) = 0;
    };
}