/*
 *  Copyright (C) 2020 Metamod++ Development Team
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

#include "Common.hpp"

#if defined META_CORE
struct TraceResult;
#endif

namespace Metamod::Engine
{
    class IEdict;

    class ITraceResult
    {
    public:
        virtual ~ITraceResult() = default;

        [[nodiscard]] virtual bool getAllSolid() const = 0;
        [[nodiscard]] virtual bool getStartSolid() const = 0;
        [[nodiscard]] virtual bool getInOpen() const = 0;
        [[nodiscard]] virtual bool getInWater() const = 0;
        [[nodiscard]] virtual float getFraction() const = 0;
        [[nodiscard]] virtual const float *getEndPos() const = 0;
        [[nodiscard]] virtual float getPlaneDist() const = 0;
        [[nodiscard]] virtual const float *getPlaneNormal() const = 0;
        [[nodiscard]] virtual IEdict *getHit() const = 0;
        [[nodiscard]] virtual HitGroup getHitGroup() const = 0;

        virtual void setAllSolid(bool allSolid) = 0;
        virtual void setStartSolid(bool startSolid) = 0;
        virtual void setInOpen(bool inOpen) = 0;
        virtual void setInWater(bool inWater) = 0;
        virtual void setFraction(float fraction) = 0;
        virtual void setEndPos(const float *endPos) = 0;
        virtual void setPlaneDist(float planeDist) = 0;
        virtual void setPlaneNormal(const float *planeNormal) = 0;
        virtual void setHit(IEdict *hit) = 0;
        virtual void setHitGroup(HitGroup hitGroup) = 0;

#if defined META_CORE
        virtual operator ::TraceResult *() const = 0;
#endif
    };
} // namespace SPMod::Engine