/*
 *  Copyright (C) 2020 SPMod Development Team
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

namespace SPMod::Engine
{
    /**
     *  Hit group.
     */
    enum class HitGroup : std::uint8_t
    {
        Generic = 0,  /**< Generic */
        Head = 1,     /**< Head */
        Chest = 2,    /**< Chest */
        Stomach = 3,  /**< Stomach */
        LeftArm = 4,  /**< Left arm */
        RightArm = 5, /**< Right arm */
        LeftLeg = 6,  /**< Left leg */
        RightLeg = 7  /**< Right leg */
    };

    /**
     * Server command callback.
     */
    using ServerCmdCallback = void (*)();
} // namespace SPMod::Engine