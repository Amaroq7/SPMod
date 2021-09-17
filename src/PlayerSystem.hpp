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

#include <IPlayerSystem.hpp>
#include "AnubisInit.hpp"
#include "MenuSystem.hpp"
#include "AccessSystem.hpp"

#include <anubis/observer_ptr.hpp>

class Player : public IPlayer
{
public:
    Player() = delete;
    ~Player() override = default;
    Player(nstd::observer_ptr<Anubis::Engine::IEdict> edict);

    // IPlayer
    std::string_view getName() const override;
    std::string_view getIPAddress() const override;
    std::string_view getSteamID() const override;
    Anubis::Engine::UserID getUserId() const override;
    std::uint32_t getIndex() const override;
    bool isAlive() const override;
    bool isConnected() const override;
    bool isFake() const override;
    bool isHLTV() const override;
    bool isInGame() const override;
    void closeMenu() override;
    nstd::observer_ptr<IMenu> getMenu() const override;
    std::uint32_t getMenuPage() const override;
    Anubis::Game::IBasePlayer *basePlayer() const override;
    Anubis::Engine::IEdict *edict() const override;
    bool sendMsg(TextMsgDest msgDest, std::string_view message) const override;
    bool hasAccess(std::string_view permission) const override;
    bool attachGroup(std::weak_ptr<IAccessGroup> group) override;
    void removeGroup(std::weak_ptr<IAccessGroup> group) override;
    bool attachPermission(std::string_view permission) override;
    void removePermission(std::string_view permission) override;

    // Player
    void setName(std::string_view newname);

    void setMenu(Menu *menu);
    void setMenuPage(std::uint32_t page);

    void connect(std::string_view name, std::string_view ip);

    void disconnect();
    void putInServer();
    void authorize(std::string_view authid);

private:
    bool m_connected = false;
    bool m_inGame = false;
    std::uint32_t m_userID = 0;
    std::string m_name;
    std::string m_ip;
    std::string m_steamID;

    nstd::observer_ptr<Menu> m_menu;
    std::uint32_t m_menuPage = 0;
    Metamod::Game::IBasePlayer *m_basePlayer;
    nstd::observer_ptr<Anubis::Engine::IEdict> m_edict;

    std::unordered_map<std::string, std::weak_ptr<IAccessGroup>> m_groups;
    std::unordered_set<std::string> m_permissions;
    std::unordered_set<std::string> m_excludedGroupPermissions;
};

class PlayerMngr : public IPlayerMngr
{
public:
    PlayerMngr();
    ~PlayerMngr() = default;

    // IPlayerManager
    Player *getPlayer(std::uint32_t index) const override;
    Player *getPlayer(const Metamod::Engine::IEdict *edict) const override;
    std::uint32_t getMaxClients() const override;
    std::uint32_t getNumPlayers() const override;

    // PlayerManager
    bool ClientConnect(Metamod::Engine::IEdict *pEntity, std::string_view pszName, std::string_view pszAddress, std::string &szRejectReason);
    void ClientPutInServer(Metamod::Engine::IEdict *pEntity);
    void ClientUserInfoChanged(Metamod::Engine::IEdict *pEntity, Metamod::Engine::InfoBuffer infoBuffer);
    void ClientDrop(Metamod::Engine::IEdict *pEntity, bool crash, std::string_view string);
    void StartFrame();
    void ServerActivate();
    void ServerDeactivate();

    static inline std::uint32_t m_playersNum = 0;

private:
    void _setMaxClients();
    void _initPlayers();

    std::vector<Player *> m_playersToAuth;
    float m_nextAuthCheck = 0.0f;

    std::array<std::unique_ptr<Player>, MAX_PLAYERS + 1> m_players;
    std::uint32_t m_maxClients = 0;
};
