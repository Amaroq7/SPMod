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
     *  Message destination.
     */
    enum class MessageDest : std::uint8_t
    {
        BROADCAST = 0,      /**< Unreliable to all */
        ONE = 1,            /**< Reliable to one (msg_entity) */
        ALL = 2,            /**< Reliable to all */
        INIT = 3,           /**< Write to the init string */
        PVS = 4,            /**< Ents in PVS of org */
        PAS = 5,            /**< Ents in PAS of org */
        PVS_R = 6,          /**< Reliable to PVS */
        PAS_R = 7,          /**< Reliable to PAS */
        ONE_UNRELIABLE = 8, /**< Send to one client, but don't put in reliable stream, put in unreliable datagram */
        SPEC = 9,           /**< Sends to all spectator proxies */
    };

    /**
     * Server command callback.
     */
    using ServerCmdCallback = void (*)();
} // namespace SPMod::Engine