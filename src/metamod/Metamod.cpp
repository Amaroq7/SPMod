/*
 *  Copyright (C) 2020 SPMod Development Team

 *  This file is part of SPMod.

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

namespace SPMod::Metamod
{
    Metamod::Metamod() : m_funcs(std::make_unique<Funcs>()) {}
    Funcs *Metamod::getFuncs() const
    {
        return m_funcs.get();
    }
} // namespace SPMod::Metamod
