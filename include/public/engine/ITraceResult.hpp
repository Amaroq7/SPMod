/*
 *  Copyright (C) 2020 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  SPMod is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with SPMod.  If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once

namespace SPMod::Engine
{
    class IEdict;

    class ITraceResult
    {
    public:
        virtual ~ITraceResult() = default;

        virtual bool getAllSolid() const = 0;
        virtual bool getStartSolid() const = 0;
        virtual bool getInOpen() const = 0;
        virtual bool getInWater() const = 0;
        virtual float getFraction() const = 0;
        virtual const float *getEndPos() const = 0;
        virtual float getPlaneDist() const = 0;
        virtual const float *getPlaneNormal() const = 0;
        virtual IEdict *getHit() const = 0;
        virtual HitGroup getHitGroup() const = 0;

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
    };
} // namespace SPMod::Engine