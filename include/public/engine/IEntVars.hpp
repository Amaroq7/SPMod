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
    enum class EntFlags : std::uint32_t
    {
        FLY = (1u << 0),  // Changes the SV_Movestep() behavior to not need to be on ground
        SWIM = (1u << 1), // Changes the SV_Movestep() behavior to not need to be on ground (but stay in water)
        CONVEYOR = (1u << 2),
        CLIENT = (1u << 3),
        INWATER = (1u << 4),
        MONSTER = (1u << 5),
        GODMODE = (1u << 6),
        NOTARGET = (1u << 7),
        SKIPLOCALHOST = (1u << 8),  // Don't send entity to local host, it's predicting this entity itself
        ONGROUND = (1u << 9),       // At rest / on the ground
        PARTIALGROUND = (1u << 10), // not all corners are valid
        WATERJUMP = (1u << 11),     // player jumping out of water
        FROZEN = (1u << 12),        // Player is frozen for 3rd person camera
        FAKECLIENT = (1u << 13),    // JAC: fake client, simulated server side; don't send network messages to them
        DUCKING = (1u << 14),       // Player flag -- Player is fully crouched
        FLOAT = (1u << 15),         // Apply floating force to this entity when in water
        GRAPHED = (1u << 16),       // worldgraph has this ent listed as something that blocks a connection

        IMMUNE_WATER = (1u << 17),
        IMMUNE_SLIME = (1u << 18),
        IMMUNE_LAVA = (1u << 19),

        PROXY = (1u << 20),       // This is a spectator proxy
        ALWAYSTHINK = (1u << 21), // Brush model flag -- call think every frame regardless of nextthink - ltime (for
                                  // constantly changing velocity/path)
        BASEVELOCITY =
            (1u << 22), // Base velocity has been applied this frame (used to convert base velocity into momentum)
        MONSTERCLIP = (1u << 23),  // Only collide in with monsters who have FL_MONSTERCLIP set
        ONTRAIN = (1u << 24),      // Player is _controlling_ a train, so movement commands should be ignored on client
                                   // during prediction.
        WORLDBRUSH = (1u << 25),   // Not moveable/removeable brush entity (really part of the world, but represented
                                   // as an entity for transparency or something)
        SPECTATOR = (1u << 26),    // This client is a spectator, don't run touch functions, etc.
        CUSTOMENTITY = (1u << 29), // This is a custom entity
        KILLME = (1u << 30),       // This entity is marked for death -- This allows the engine to kill ents at the
                                   // appropriate time
        DORMANT = (1u << 31)       // Entity is dormant, no updates to client
    };

    class IEntVars
    {
    public:
        virtual ~IEntVars() = default;

        virtual EntFlags getFlags() const = 0;
        virtual void setFlags(EntFlags flags) = 0;

        virtual std::uint32_t getIndex() const = 0;
    };
} // namespace SPMod::Engine