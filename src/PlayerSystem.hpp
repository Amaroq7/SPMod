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

#include "spmod.hpp"

class Menu;

#if defined SP_MSVC
    #pragma warning(push)
    #pragma warning(disable : 4250) // inheritance via dominance warning
#endif
class Player : public Edict, public IPlayer
{
public:
    Player() = delete;
    ~Player() = default;
    Player(const Edict *edict);

    // IPlayer
    std::string_view getName() const override;
    std::string_view getIPAddress() const override;
    std::string_view getSteamID() const override;
    std::uint32_t getUserId() const override;
    bool isAlive() const override;
    bool isConnected() const override;
    bool isFake() const override;
    bool isHLTV() const override;
    bool isInGame() const override;
    void closeMenu() override;
    Menu *getMenu() const override;
    std::uint32_t getMenuPage() const override;

    // Player
    void setName(std::string_view newname);

    void setMenu(Menu *menu);
    void setMenuPage(std::uint32_t page);

    void connect(std::string_view name, std::string_view ip);

    void disconnect();
    void putInServer();
    void authorize(std::string_view authid);

private:
    bool m_connected;
    bool m_inGame;
    std::uint32_t m_userID;
    std::string m_name;
    std::string m_ip;
    std::string m_steamID;

    Menu *m_menu;
    std::uint32_t m_menuPage;
};
#if defined SP_MSVC
    #pragma warning(pop)
#endif

class PlayerMngr : public IPlayerMngr
{
public:
    PlayerMngr() = default;
    ~PlayerMngr() = default;

    // IPlayerManager
    Player *getPlayer(std::uint32_t index) const override;
    Player *getPlayer(const IEdict *edict) const override;
    std::uint32_t getMaxClients() const override;
    std::uint32_t getNumPlayers() const override;

    // PlayerManager
    Player *getPlayer(edict_t *edict) const;

    bool ClientConnect(edict_t *pEntity,
                       std::string_view pszName,
                       std::string_view pszAddress,
                       char szRejectReason[128]);
    void ClientConnectPost(edict_t *pEntity, std::string_view pszName, std::string_view pszAddress);
    void ClientPutInServer(edict_t *pEntity);
    void ClientPutInServerPost(edict_t *pEntity);
    void ClientUserInfoChangedPost(edict_t *pEntity, char *infobuffer);
    void StartFramePost();
    void ServerActivatePost(edict_t *pEdictList, std::uint32_t clientMax);

    static inline std::uint32_t m_playersNum;

private:
    void _setMaxClients(std::uint32_t maxClients);
    void _initPlayers(edict_t *edictList);

    std::vector<Player *> m_playersToAuth;
    float m_nextAuthCheck = 0.0f;

    std::array<std::unique_ptr<Player>, MAX_PLAYERS + 1> m_players;
    std::uint32_t m_maxClients;
};
