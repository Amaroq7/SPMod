/*
 *  Copyright (C) 2018-2019 SPMod Development Team
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

#include "spmod.hpp"

Edict::Edict(edict_t *edict) : m_edict(edict)
{
}

Edict::Edict(int index) : m_edict(INDEXENT(index))
{
}

int Edict::getIndex() const
{
    return ENTINDEX(m_edict);
}

const char *Edict::getClassName() const
{
    return STRING(m_edict->v.classname);
}

void Edict::getOrigin(float *origin) const
{
    const vec3_t &edPos = m_edict->v.origin;
    edPos.CopyToArray(origin);
}

void Edict::getVelocity(float *velocity) const
{
    const vec3_t &edVelocity = m_edict->v.velocity;
    edVelocity.CopyToArray(velocity);
}

void Edict::setVelocity(const float *velocity)
{
    m_edict->v.velocity = const_cast<float *>(velocity);
}

float Edict::getHealth() const
{
    return m_edict->v.health;
}

void Edict::setHealth(float health)
{
    m_edict->v.health = health;
}

void *Edict::getPrivateData() const
{
    return m_edict->pvPrivateData;
}

edict_t *Edict::getInternalEdict() const
{
    return m_edict;
}