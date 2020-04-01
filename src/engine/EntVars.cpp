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
    EntVars::EntVars(entvars_t *entvars) : m_entVars(entvars) {}

    EntFlags EntVars::getFlags() const
    {
        return static_cast<EntFlags>(m_entVars->flags);
    }

    void EntVars::setFlags(EntFlags flags)
    {
        m_entVars->flags = static_cast<int>(flags);
    }

    std::uint32_t EntVars::getIndex() const
    {
        return static_cast<std::uint32_t>(ENTINDEX(ENT(m_entVars)));
    }

    EntVars::operator entvars_t *() const
    {
        return m_entVars;
    }
} // namespace SPMod::Engine