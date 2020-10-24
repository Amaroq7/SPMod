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

#include "spmod.hpp"

Player::Player(Engine::Edict *edict) : m_basePlayer(edict->getBasePlayer()), m_edict(edict) {}

std::string_view Player::getName() const
{
    return m_name;
}

std::string_view Player::getIPAddress() const
{
    return m_ip;
}

std::string_view Player::getSteamID() const
{
    return m_steamID;
}

std::uint32_t Player::getIndex() const
{
    return m_edict->getIndex();
}

bool Player::isAlive() const
{
    return m_basePlayer->isAlive();
}

void Player::setName(std::string_view newname)
{
    m_name = newname;
}

Menu *Player::getMenu() const
{
    return m_menu;
}

void Player::closeMenu()
{
    if (!m_menu)
    {
        return;
    }

    if (m_menu->getStyle() == IMenu::Style::Item)
        m_menu->execExitHandler(this);

    static std::string_view menu("\n");
    gSPGlobal->getUtils()->ShowMenu(m_edict, 0, 0, menu);

    setMenu(nullptr);
}

void Player::setMenu(Menu *menu)
{
    m_menu = menu;
}

std::uint32_t Player::getMenuPage() const
{
    return m_menuPage;
}

void Player::setMenuPage(std::uint32_t page)
{
    m_menuPage = page;
}

void Player::connect(std::string_view name, std::string_view ip)
{
    m_name = name;
    m_ip = ip;
    m_connected = true;
    m_userID = GETPLAYERUSERID(*m_edict);
}

void Player::disconnect()
{
    m_connected = false;
    m_inGame = false;

    closeMenu();

    m_ip.clear();
    m_name.clear();
    m_steamID.clear();
}

void Player::putInServer()
{
    m_inGame = true;
}

void Player::authorize(std::string_view authid)
{
    m_steamID = authid;
}

std::uint32_t Player::getUserId() const
{
    return m_userID;
}

bool Player::isConnected() const
{
    return m_connected;
}

bool Player::isFake() const
{
    return (m_edict->getEntVars()->getFlags() & Engine::EntFlags::FAKECLIENT || m_steamID == "BOT");
}

bool Player::isHLTV() const
{
    return (m_edict->getEntVars()->getFlags() & Engine::EntFlags::PROXY || m_steamID == "HLTV");
}

bool Player::isInGame() const
{
    return m_inGame;
}

IBasePlayer *Player::basePlayer() const
{
    if (gSPGlobal->getModType() == ModType::Valve)
    {
        return m_basePlayer;
    }

    return nullptr;
}

Engine::Edict *Player::edict() const
{
    return m_edict;
}

Player *PlayerMngr::getPlayer(std::uint32_t index) const
{
    try
    {
        return m_players.at(index).get();
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return nullptr;
    }
}

Player *PlayerMngr::getPlayer(const Engine::IEdict *edict) const
{
    try
    {
        return m_players.at(edict->getIndex()).get();
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return nullptr;
    }
}

std::uint32_t PlayerMngr::getMaxClients() const
{
    return m_maxClients;
}

std::uint32_t PlayerMngr::getNumPlayers() const
{
    return m_playersNum;
}

void PlayerMngr::_initPlayers()
{
    for (std::size_t i = 1; i <= m_maxClients; i++)
    {
        Engine::Edict *spEdict = gSPGlobal->getEngine()->getEdict(i);
        m_players.at(i) = std::make_unique<Player>(spEdict);
    }
}

Player *PlayerMngr::getPlayer(edict_t *edict) const
{
    try
    {
        return m_players.at(ENTINDEX(edict)).get();
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return nullptr;
    }
}

void PlayerMngr::_setMaxClients(std::uint32_t maxClients)
{
    m_maxClients = maxClients;
}

bool PlayerMngr::ClientConnect(edict_t *pEntity,
                               std::string_view pszName,
                               std::string_view pszAddress,
                               char szRejectReason[128])
{
    using sf = IForward::StringFlags;

    Forward *fwdPlrConnect = gSPGlobal->getForwardManager()->getForward(ForwardMngr::FWD_PLAYER_CONNECT);

    std::int32_t result;
    fwdPlrConnect->pushInt(ENTINDEX(pEntity));
    fwdPlrConnect->pushString(pszName);
    fwdPlrConnect->pushString(pszAddress);
    fwdPlrConnect->pushString(szRejectReason, 128, sf::Copy | sf::Utf8, true);
    fwdPlrConnect->execFunc(&result);

    if (static_cast<IForward::ReturnValue>(result) == IForward::ReturnValue::Stop)
    {
        return false;
    }

    return true;
}

void PlayerMngr::ClientConnectPost(edict_t *pEntity, std::string_view pszName, std::string_view pszAddress)
{
    Player *plr = getPlayer(pEntity);
    plr->connect(pszName, pszAddress);

    PlayerMngr::m_playersNum++;

    std::string_view authid(GETPLAYERAUTHID(pEntity));

    if (authid.empty() || authid == "STEAM_ID_PENDING")
        m_playersToAuth.emplace_back(plr);
    else
        plr->authorize(authid);

    Forward *fwdPlrConnectPost = gSPGlobal->getForwardManager()->getForward(ForwardMngr::FWD_PLAYER_CONNECTED);
    fwdPlrConnectPost->pushInt(ENTINDEX(pEntity));
    fwdPlrConnectPost->pushString(pszName);
    fwdPlrConnectPost->pushString(pszAddress);
    fwdPlrConnectPost->execFunc(nullptr);
}

void PlayerMngr::ClientPutInServer(edict_t *pEntity)
{
    Player *plr = getPlayer(pEntity);

    Forward *forward = gSPGlobal->getForwardManager()->getForward(ForwardMngr::FWD_PLAYER_ENTER);
    forward->pushInt(plr->getIndex());
    forward->execFunc(nullptr);
}

void PlayerMngr::ClientPutInServerPost(edict_t *pEntity)
{
    Player *plr = getPlayer(pEntity);
    plr->putInServer();

    Forward *forward = gSPGlobal->getForwardManager()->getForward(ForwardMngr::FWD_PLAYER_ENTERED);
    forward->pushInt(plr->getIndex());
    forward->execFunc(nullptr);
}

void PlayerMngr::ClientUserInfoChangedPost(edict_t *pEntity, char *infobuffer)
{
    getPlayer(pEntity)->setName(INFOKEY_VALUE(infobuffer, "name"));
}

void PlayerMngr::StartFramePost()
{
    if (m_nextAuthCheck <= gpGlobals->time && !m_playersToAuth.empty())
    {
        m_nextAuthCheck = gpGlobals->time + 0.5f;

        auto iter = m_playersToAuth.begin();
        while (iter != m_playersToAuth.end())
        {
            Player *plr = *iter;
            std::string_view authid(GETPLAYERAUTHID(*plr->edict()));
            if (!authid.empty() && authid == "STEAM_ID_PENDING")
            {
                plr->authorize(authid);
                iter = m_playersToAuth.erase(iter);
            }
            else
                ++iter;
        }
    }
}

void PlayerMngr::ServerActivatePost(edict_t *pEdictList [[maybe_unused]], std::uint32_t clientMax)
{
    _setMaxClients(clientMax);
    _initPlayers();
}

void PlayerMngr::ServerDeactivatePost()
{
    // Reset player menus
    for (std::size_t i = 1; i <= m_maxClients; i++)
    {
        m_players.at(i)->setMenu(nullptr);
        m_players.at(i)->setMenuPage(0);
    }
}