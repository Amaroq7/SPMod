/*
 *  Copyright (C) 2019-2020 SPMod Development Team
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

namespace SPMod
{
    class IBaseEntity;
    class IBasePlayer;
} // namespace SPMod

namespace SPMod::Engine
{
    class IEdict
    {
    public:
        virtual ~IEdict() = default;

        /**
         * @brief Returns edict's index.
         *
         * @return Edict's index.
         */
        virtual std::uint32_t getIndex() const = 0;

        /**
         * @brief Returns edict's index.
         *
         * @return Edict's index.
         */
        virtual std::uint32_t getSerialNumber() const = 0;

        /**
         * @brief Checks if edict is freed.
         *
         * @return True if edict is freed, false otherwise.
         */
        virtual bool isValid() const = 0;

        /**
         * @brief Returns entity.
         *
         * @return Edict's base entity representation.
         */
        virtual SPMod::IBaseEntity *getBaseEntity() = 0;

        /**
         * @brief Returns player entity.
         *
         * @return Edict's player entity representation.
         */
        virtual SPMod::IBasePlayer *getBasePlayer() = 0;

        /**
         * @brief Returns entity's variables.
         *
         * @return Enitity's variables.
         */
        virtual IEntVars *getEntVars() const = 0;
    };
} // namespace SPMod::Engine
