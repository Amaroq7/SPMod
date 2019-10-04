/*
 *  Copyright (C) 2018 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
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

    class IEdict;
    class IMenu;

    class IPlayer : public virtual IEdict
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

        virtual IMenu *getMenu() const = 0;
        virtual int getMenuPage() const = 0;
        virtual void closeMenu() = 0;

    protected:
        virtual ~IPlayer() = default;
    };

    class IPlayerMngr : public ISPModInterface
    {
    public:
        static constexpr std::uint16_t MAJOR_VERSION = 0;
        static constexpr std::uint16_t MINOR_VERSION = 0;

        static constexpr std::uint32_t VERSION = (MAJOR_VERSION << 16 | MINOR_VERSION);
        /**
         * @brief Gets interface's name.
         *
         * @return        Interface's name.
         */
        const char *getName() const override
        {
            return "IPlayerMngr";
        }

        /**
         * @brief Gets interface's version.
         *
         * @note The first 16 most significant bits represent major version, the rest represent minor version.
         *
         * @return        Interface's version.
         */
        std::uint32_t getVersion() const override
        {
            return VERSION;
        }

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
        virtual IPlayer *getPlayer(IEdict *edict) const = 0;

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
} // namespace SPMod
