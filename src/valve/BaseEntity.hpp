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

#if defined SP_MSVC
    #pragma warning(push)
    #pragma warning(disable:4458)
#endif
#include <rehlds/dlls/cbase.h>
#if defined SP_MSVC
    #pragma warning(pop)
#endif

class Edict;

namespace Valve
{
    class BaseEntity : public virtual IBaseEntity
    {
    public:
        BaseEntity() = delete;
        BaseEntity(const Engine::Edict *edict);
        virtual ~BaseEntity() = default;

        Engine::IEdict *edict() const final override;
        void remove() override;
        bool isAlive() const override;
        std::string_view getTeam() const override;
        int takeHealth(float flHealth, int bitsDamageType) const override;
        int takeDamage(Engine::IEntVars *pevInflictor,
                       Engine::IEntVars *pevAttacker,
                       float flDamage,
                       int bitsDamageType) override;

        void setCallHooks(bool call = false);

    protected:
        Engine::Edict *m_edict;
        bool m_callHooks;

    private:
        operator CBaseEntity *() const;
    };
} // namespace Valve