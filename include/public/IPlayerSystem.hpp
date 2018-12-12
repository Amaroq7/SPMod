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

    class IPlayerListener
    {
    public:
        /*
         * @brief Called when a client connects to the server.
         *
         * @param entity    Client's edict_t structure.
         * @param name      Client's name.
         * @param address   Client's IP address.
         * @param reason    Buffer to write a reason that will be displayed to the client when they get rejected.
         *
         * @return          True to let client join or false to reject them.
         */
        virtual bool OnClientConnect(edict_t *entity [[maybe_unused]],
                                     const char *name [[maybe_unused]],
                                     const char *address [[maybe_unused]],
                                     char reason [[maybe_unused]] [128])
        {
            return true;
        }

        /*
         * @brief Called when a client has connected to the server.
         *
         * @param player        Player object.
         *
         * @noreturn
         */
        virtual void OnClientConnected(IPlayer *player [[maybe_unused]])
        {
        }

        /*
         * @brief Called when a client enters the game.
         *
         * @param player        Player object.
         *
         * @noreturn
         */
        virtual void OnClientPutInServer(IPlayer *player [[maybe_unused]])
        {
        }

        /*
         * @brief Called when a client disconnects from the server.
         *
         * @param player        Player object.
         * @param crash         True if client crashed, false otherwise.
         * @param reason        Reason why the client is disconnecting.
         *
         * @noreturn
         */
        virtual void OnClientDisconnect(IPlayer *player [[maybe_unused]],
                                        bool crash [[maybe_unused]],
                                        const char *reason [[maybe_unused]])
        {
        }

        /*
         * @brief Called when a client disconnected from the server.
         *
         * @param player        Player object.
         * @param crash         True if client crashed, false otherwise.
         * @param reason        Reason why the client has been disconnected.
         *
         * @noreturn
         */
        virtual void OnClientDisconnected(IPlayer *player [[maybe_unused]],
                                          bool crash [[maybe_unused]],
                                          const char *reason [[maybe_unused]])
        {
        }

    protected:
        virtual ~IPlayerListener() = default;
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

        /**
         * @brief Adds player listener.
         *
         * @param listener  Pointer to player listener instance.
         *
         * @noreturn
         */
        virtual void addPlayerListener(IPlayerListener *listener) = 0;

        /**
         * @brief Removes player listener.
         *
         * @param listener  Pointer to player listener instance.
         *
         * @noreturn
         */
        virtual void removePlayerListener(IPlayerListener *listener) = 0;

    protected:
        virtual ~IPlayerMngr() = default;
    };
}
