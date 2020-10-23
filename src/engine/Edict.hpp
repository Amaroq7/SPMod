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

#pragma once

#include "../spmod.hpp"

namespace SPMod::Engine
{
    class Edict final : public IEdict
    {
    public:
        Edict() = delete;
        Edict(edict_t *edict);
        ~Edict() = default;

        // IEdict
        std::uint32_t getIndex() const override;
        std::uint32_t getSerialNumber() const override;
        bool isValid() const override;
        IBaseEntity *getBaseEntity() override;
        IBasePlayer *getBasePlayer() override;

        EntVars *getEntVars() const override;

        // Edict
        operator edict_t *() const;

    private:
        bool _isValid() const;

        template<typename T, typename = std::enable_if_t<std::is_base_of_v<IBaseEntity, T>>>
        T *getEntity()
        {
            if (!_isValid())
            {
                return nullptr;
            }

            if (m_serialNumber != static_cast<std::uint32_t>(m_edict->serialnumber))
            {
                m_entities.clear();
                m_serialNumber = static_cast<std::uint32_t>(m_edict->serialnumber);
            }
            else
            {
                for (const auto &entity : m_entities)
                {
                    if (auto entityBase = dynamic_cast<T *>(entity.get()); entityBase)
                    {
                        return entityBase;
                    }
                }
            }

            return dynamic_cast<T *>(m_entities.emplace_back(std::make_unique<T>(this)).get());
        }

    private:
        edict_t *m_edict = nullptr;
        std::vector<std::unique_ptr<IBaseEntity>> m_entities;
        std::unique_ptr<EntVars> m_entVars;
        std::uint32_t m_serialNumber = 0;
    };
} // namespace SPMod::Engine