/*
 *  Copyright (C) 2018-2020 SPMod Development Team
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

#include "../spmod.hpp"

namespace SPMod::Engine
{
    Edict::Edict(edict_t *edict)
        : m_edict(edict), m_entVars(std::make_unique<EntVars>(VARS(edict))),
          m_serialNumber(static_cast<std::uint32_t>(edict->serialnumber))
    {
    }

    std::uint32_t Edict::getIndex() const
    {
        return ENTINDEX(m_edict);
    }

    std::uint32_t Edict::getSerialNumber() const
    {
        return static_cast<std::uint32_t>(m_edict->serialnumber);
    }

    bool Edict::isValid() const
    {
        return !FNullEnt(m_edict) && !m_edict->free;
    }

    IBaseEntity *Edict::getBaseEntity()
    {
        switch (gSPGlobal->getModName())
        {
            case ModName::Valve:
                return getEntity<Valve::BaseEntity>();
            default:
                return nullptr;
        }
    }

    IBasePlayer *Edict::getBasePlayer()
    {
        switch (gSPGlobal->getModName())
        {
            case ModName::Valve:
                return getEntity<Valve::BasePlayer>();
            default:
                return nullptr;
        }
    }

    EntVars *Edict::getEntVars() const
    {
        return m_entVars.get();
    }

    Edict::operator edict_t *() const
    {
        return m_edict;
    }
} // namespace SPMod::Engine