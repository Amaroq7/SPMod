/*
 *  Copyright (C) 2018-2020 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
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

#pragma once

#include "StandardHeaders.hpp"
#include "IInterface.hpp"
#include "IHookChains.hpp"

namespace Metamod
{
    namespace Engine
    {
        class IEdict;
    }
    namespace Game
    {
        class IBasePlayer;
    }
}

namespace SPMod
{
    class IMenu;

    enum class TextMsgDest : std::uint8_t
    {
        Notify = 1,
        Console,
        Chat,
        Center,
        Radio
    };

    class IPlayer
    {
    public:
        virtual ~IPlayer() = default;

        /**
         * @brief Returns the player's name.
         *
         * @return      Player's name.
         */
        virtual std::string_view getName() const = 0;

        /**
         * @brief Returns the player's IP address.
         *
         * @return      Player's IP address.
         */
        virtual std::string_view getIPAddress() const = 0;

        /**
         * @brief Returns the players's Steam ID.
         *
         * @return      Player's SteamID.
         */
        virtual std::string_view getSteamID() const = 0;

        /**
         * @brief Returns the player's userid.
         *
         * @return      Player's userid.
         */
        virtual std::uint32_t getUserId() const = 0;

        /**
         * @brief Returns the player's index.
         *
         * @return      Player's index.
         */
        virtual std::uint32_t getIndex() const = 0;

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
        virtual std::uint32_t getMenuPage() const = 0;
        virtual void closeMenu() = 0;

        virtual Metamod::Game::IBasePlayer *basePlayer() const = 0;
        virtual Metamod::Engine::IEdict *edict() const = 0;

        virtual bool sendMsg(TextMsgDest msgDest, std::string_view message) const = 0;
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
        std::string_view getName() const override
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

        virtual ~IPlayerMngr() = default;

        /**
         * @brief Returns IPlayer object by its client index.
         *
         * @note Index starts at 1.
         *
         * @param index     Index of the client.
         *
         * @return          IPlayer object, nullptr if out of range.
         */
        virtual IPlayer *getPlayer(std::uint32_t index) const = 0;

        /**
         * @brief Returns IPlayer object by its edict_t pointer.
         *
         * @param edict     Edict instance of the client.
         *
         * @return          IPlayer object, nullptr if out of range.
         */
        virtual IPlayer *getPlayer(const Metamod::Engine::IEdict *edict) const = 0;

        /**
         * @brief Returns the maximum number of clients.
         *
         * @return          Maximum number of clients.
         */
        virtual std::uint32_t getMaxClients() const = 0;

        /**
         * @brief Returns the number of players currently connected.
         *
         * @return          Number of connected clients.
         */
        virtual std::uint32_t getNumPlayers() const = 0;
    };
} // namespace SPMod
