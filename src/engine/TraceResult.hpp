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

#include "../spmod.hpp"

namespace SPMod::Engine
{
    class TraceResult : public ITraceResult
    {
    public:
        TraceResult();
        TraceResult(const TraceResult &other) = delete;
        TraceResult(TraceResult &&other) = delete;
        TraceResult(::TraceResult *traceResult);
        ~TraceResult() = default;

        bool getAllSolid() const;
        bool getStartSolid() const;
        bool getInOpen() const;
        bool getInWater() const;
        float getFraction() const;
        const float *getEndPos() const;
        float getPlaneDist() const;
        const float *getPlaneNormal() const;
        Edict *getHit() const;
        HitGroup getHitGroup() const;

        void setAllSolid(bool allSolid);
        void setStartSolid(bool startSolid);
        void setInOpen(bool inOpen);
        void setInWater(bool inWater);
        void setFraction(float fraction);
        void setEndPos(const float *endPos);
        void setPlaneDist(float planeDist);
        void setPlaneNormal(const float *planeNormal);
        void setHit(IEdict *hit);
        void setHitGroup(HitGroup hitGroup);

        operator ::TraceResult *() const;

    private:
        std::variant<::TraceResult *, std::unique_ptr<::TraceResult>> m_traceResult;
    };
} // namespace SPMod::Engine