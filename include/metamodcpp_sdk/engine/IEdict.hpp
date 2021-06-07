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

#include <cinttypes>
#include <cstddef>

#if defined META_CORE
typedef struct edict_s edict_t;
#endif

namespace Metamod::Engine
{
    class IEntVars;

    class IEdict
    {
    public:
        virtual ~IEdict() = default;

        /**
         * @brief Returns edict's index.
         *
         * @return Edict's index.
         */
        [[nodiscard]] virtual std::uint32_t getIndex() const = 0;

        /**
         * @brief Returns edict's index.
         *
         * @return Edict's index.
         */
        [[nodiscard]] virtual std::uint32_t getSerialNumber() const = 0;

        /**
         * @brief Checks if edict is freed.
         *
         * @return True if edict is freed, false otherwise.
         */
        [[nodiscard]] virtual bool isValid() const = 0;

        /**
         * @brief Returns entity's variables.
         *
         * @return Enitity's variables.
         */
        [[nodiscard]] virtual IEntVars *getEntVars() const = 0;

#if defined META_CORE
        [[nodiscard]] virtual void *getPrivateData() const = 0;
        virtual explicit operator edict_t *() const = 0;
#endif
    };
}