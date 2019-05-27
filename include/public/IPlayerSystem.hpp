/*
 *  Copyright (C) 2018 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <cstddef>

#include <archtypes.h>

#if !defined vec_t
    using vec_t = float;
#endif

#if !defined sqrt
    #include <cmath>
    #define sqrt std::sqrt
#endif

#include <vector.h>

#if !defined vec3_t
    #define vec3_t Vector
#endif

#if !defined string_t
    using string_t = unsigned int;
#endif

#if !defined byte
    using byte = unsigned char;
#endif

#include <const.h>
#include <edict.h>

namespace SPMod
{
    /**
     * @brief Maximum number of supported players.
     */
    constexpr unsigned int MAX_PLAYERS = 32U;

    class IPlayer
    {
    public:
        /**
         * @brief Returns the player's name.
         *
         * @return      Player's name.
         */
        virtual const char *getName() const = 0;

        /**
         * @brief Returns the player's IP address.
         *
         * @return      Player's IP address.
         */
        virtual const char *getIPAddress() const = 0;

        /**
         * @brief Returns the players's Steam ID.
         *
         * @return      Player's SteamID.
         */
        virtual const char *getSteamID() const = 0;

        /**
         * @brief Returns the player's edict_t structure.
         *
         * @return      Player's edict_t structure.
         */
        virtual edict_t *getEdict() const = 0;

        /**
         * @brief Returns the player's index.
         *
         * @return      Player's index.
         */
        virtual unsigned int getIndex() const = 0;

        /**
         * @brief Returns the player's userid.
         *
         * @return      Player's Userid.
         */
        virtual int getUserId() const = 0;

        /**
         * @brief Returns if the player is alive.
         *
         * @return      True if connected, false otherwise.
         */
        virtual bool isAlive() const = 0;

        /**
         * @brief Returns if the player is connected.
         *
         * @return      True if connected, false otherwise.
         */
        virtual bool isConnected() const = 0;

        /**
         * @brief Returns if the player is a fake player.
         *
         * @return      True if a fake player, false otherwise.
         */
        virtual bool isFake() const = 0;

        /**
         * @brief Returns if the player is the HLTV.
         *
         * @return      True if the HLTV, false otherwise.
         */
        virtual bool isHLTV() const = 0;

        /**
         * @brief Returns if the player is in game.
         *
         * @return      True if in game, false otherwise.
         */
        virtual bool isInGame() const = 0;

    protected:
        virtual ~IPlayer() = default;
    };

    class IPlayerMngr
    {
    public:
        /**
         * @brief Returns IPlayer object by its client index.
         *
         * @note Index starts at 1.
         *
         * @param index     Index of the client.
         *
         * @return          IPlayer object, nullptr if out of range.
         */
        virtual IPlayer *getPlayer(int index) const = 0;

        /**
         * @brief Returns IPlayer object by its edict_t pointer.
         *
         * @param edict     edict_t structure of the client.
         *
         * @return          IPlayer object, nullptr if out of range.
         */
        virtual IPlayer *getPlayer(edict_t *edict) const = 0;

        /**
         * @brief Returns the maximum number of clients.
         *
         * @return          Maximum number of clients.
         */
        virtual unsigned int getMaxClients() const = 0;

        /**
         * @brief Returns the number of players currently connected.
         *
         * @return          Number of connected clients.
         */
        virtual unsigned int getNumPlayers() const = 0;

    protected:
        virtual ~IPlayerMngr() = default;
    };
}
