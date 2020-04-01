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
#include "../spmod.hpp"

namespace SPMod::Engine
{
    TraceResult::TraceResult() : m_traceResult(std::make_unique<::TraceResult>()) {}
    TraceResult::TraceResult(::TraceResult *traceResult) : m_traceResult(traceResult) {}

    bool TraceResult::getAllSolid() const
    {
        return operator ::TraceResult *()->fAllSolid == TRUE;
    }
    bool TraceResult::getStartSolid() const
    {
        return operator ::TraceResult *()->fStartSolid == TRUE;
    }
    bool TraceResult::getInOpen() const
    {
        return operator ::TraceResult *()->fInOpen == TRUE;
    }
    bool TraceResult::getInWater() const
    {
        return operator ::TraceResult *()->fInWater == TRUE;
    }
    float TraceResult::getFraction() const
    {
        return operator ::TraceResult *()->flFraction;
    }
    const float *TraceResult::getEndPos() const
    {
        return operator ::TraceResult *()->vecEndPos;
    }
    float TraceResult::getPlaneDist() const
    {
        return operator ::TraceResult *()->flPlaneDist;
    }
    const float *TraceResult::getPlaneNormal() const
    {
        return operator ::TraceResult *()->vecPlaneNormal;
    }
    Edict *TraceResult::getHit() const
    {
        return gSPGlobal->getEngine()->getEdict(ENTINDEX(operator ::TraceResult *()->pHit));
    }
    HitGroup TraceResult::getHitGroup() const
    {
        return static_cast<HitGroup>(operator ::TraceResult *()->iHitgroup);
    }

    void TraceResult::setAllSolid(bool allSolid)
    {
        operator ::TraceResult *()->fAllSolid = allSolid;
    }
    void TraceResult::setStartSolid(bool startSolid)
    {
        operator ::TraceResult *()->fStartSolid = startSolid;
    }
    void TraceResult::setInOpen(bool inOpen)
    {
        operator ::TraceResult *()->fInOpen = inOpen;
    }
    void TraceResult::setInWater(bool inWater)
    {
        operator ::TraceResult *()->fInWater = inWater;
    }
    void TraceResult::setFraction(float fraction)
    {
        operator ::TraceResult *()->flFraction = fraction;
    }
    void TraceResult::setEndPos(const float *endPos)
    {
        operator ::TraceResult *()->vecEndPos = Vector(const_cast<float *>(endPos));
    }
    void TraceResult::setPlaneDist(float planeDist)
    {
        operator ::TraceResult *()->flPlaneDist = planeDist;
    }
    void TraceResult::setPlaneNormal(const float *planeNormal)
    {
        operator ::TraceResult *()->vecEndPos = Vector(const_cast<float *>(planeNormal));
    }
    void TraceResult::setHit(IEdict *hit)
    {
        operator ::TraceResult *()->pHit = *static_cast<Edict *>(hit);
    }
    void TraceResult::setHitGroup(HitGroup hitGroup)
    {
        operator ::TraceResult *()->iHitgroup = static_cast<std::int32_t>(hitGroup);
    }

    TraceResult::operator::TraceResult *() const
    {
        return std::visit(
            [=](auto &&arg) {
                using T = std::decay_t<decltype(arg)>;

                if constexpr (std::is_same_v<T, ::TraceResult *>)
                {
                    return arg;
                }
                else
                {
                    return arg.get();
                }
            },
            m_traceResult);
    }
} // namespace SPMod::Engine