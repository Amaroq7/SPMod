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

#include "spmod.hpp"

class Menu;

class Player : public IPlayer
{
public:
    Player() = delete;
    ~Player() = default;
    Player(edict_t *edict, unsigned int index);

    // IPlayer
    const char *getName() const override;
    const char *getIPAddress() const override;
    const char *getSteamID() const override;
    edict_t *getEdict() const override;
    unsigned int getIndex() const override;
    int getUserId() const override;
    bool isAlive() const override;
    bool isConnected() const override;
    bool isFake() const override;
    bool isHLTV() const override;
    bool isInGame() const override;

    // Player
    std::string_view getNameCore() const;
    std::string_view getIPAddressCore() const;
    std::string_view getSteamIDCore() const;
    void setName(std::string_view newname);

    std::weak_ptr<Menu> getMenu() const;
    void setMenu(std::shared_ptr<Menu> menu);
    int getMenuPage() const;
    void setMenuPage(int page);

    void connect(std::string_view name,
                 std::string_view ip);

    void disconnect();
    void putInServer();
    void authorize(std::string_view authid);

private:
    edict_t *m_edict;
    unsigned int m_index;
    bool m_connected;
    bool m_inGame;
    int m_userID;
    std::string m_name;
    std::string m_ip;
    std::string m_steamID;

    std::weak_ptr<Menu> m_menu;
    int m_menuPage;
};

class PlayerMngr : public IPlayerMngr
{
public:
    PlayerMngr() = default;
    ~PlayerMngr() = default;

    // IPlayerManager
    IPlayer *getPlayer(int index) const override;
    IPlayer *getPlayer(edict_t *edict) const override;
    unsigned int getMaxClients() const override;
    unsigned int getNumPlayers() const override;

    // PlayerManager
    std::shared_ptr<Player> getPlayerCore(int index) const;
    std::shared_ptr<Player> getPlayerCore(edict_t *edict) const;

    bool ClientConnect(edict_t *pEntity,
                       const char *pszName,
                       const char *pszAddress,
                       char szRejectReason[128]);

    void ClientConnectPost(edict_t *pEntity,
                           const char *pszName,
                           const char *pszAddress);

    void ClientPutInServer(edict_t *pEntity);
    void ClientPutInServerPost(edict_t *pEntity);
    void ClientUserInfoChangedPost(edict_t *pEntity,
                                   char *infobuffer);
    void StartFramePost();
    void ServerActivatePost(edict_t *pEdictList,
                            int clientMax);

    static inline unsigned int m_playersNum;

private:
    void _setMaxClients(int maxClients);
    void _initPlayers(edict_t *edictList);

    std::vector<std::shared_ptr<Player>> m_playersToAuth;
    float m_nextAuthCheck = 0.0f;

    std::array<std::shared_ptr<Player>, MAX_PLAYERS + 1> m_players;
    unsigned int m_maxClients;
};
