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

namespace Valve
{
    BaseEntity::BaseEntity(const Engine::Edict *edict) : m_edict(const_cast<Engine::Edict *>(edict)) {}

    Engine::IEdict *BaseEntity::edict() const
    {
        return m_edict;
    }

    void BaseEntity::remove()
    {
        REMOVE_ENTITY(*m_edict);
    }

    bool BaseEntity::isAlive() const
    {
        return operator CBaseEntity *()->IsAlive() == 1;
    }

    int BaseEntity::takeHealth(float flHealth, int bitsDamageType) const
    {
        return operator CBaseEntity *()->TakeHealth(flHealth, bitsDamageType);
    }

    std::string_view BaseEntity::getTeam() const
    {
        return operator CBaseEntity *()->TeamID();
    }

    std::int32_t BaseEntity::takeDamage(Engine::IEntVars *pevInflictor,
                                        Engine::IEntVars *pevAttacker,
                                        float flDamage,
                                        std::int32_t bitsDamageType)
    {
        VTableHook *hook = gSPGlobal->getVTableManager()->getHook(IVTableHookManager::vFuncType::TakeDamage);

        // VFunc is hooked so call the original
        if (hook && !m_callHooks)
        {
            return hook->execOriginalFunc<std::int32_t, entvars_t *, entvars_t *, float, std::int32_t>(
                    operator CBaseEntity *(), *static_cast<Engine::EntVars *>(pevInflictor),
                *static_cast<Engine::EntVars *>(pevAttacker), flDamage, bitsDamageType);
        }

        return operator CBaseEntity *()->TakeDamage(*static_cast<Engine::EntVars *>(pevInflictor),
                                      *static_cast<Engine::EntVars *>(pevAttacker), flDamage,
                                      bitsDamageType);
    }

    BaseEntity::operator CBaseEntity *() const
    {
        return static_cast<CBaseEntity *>(GET_PRIVATE(*m_edict));
    }

    void BaseEntity::setCallHooks(bool call)
    {
        m_callHooks = call;
    }
} // namespace Valve