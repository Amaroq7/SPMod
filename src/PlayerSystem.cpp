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

#include "spmod.hpp"

Player::Player(edict_t *edict,
               unsigned int index) : m_edict(edict),
                                     m_index(index),
                                     m_connected(false),
                                     m_inGame(false)
{
}

std::string_view Player::getNameCore() const
{
    return m_name;
}

std::string_view Player::getIPAddressCore() const
{
    return m_ip;
}

std::string_view Player::getSteamIDCore() const
{
    return m_steamID;
}

void Player::setName(std::string_view newname)
{
    m_name = newname;
}

std::weak_ptr<Menu> Player::getMenu() const
{
    return m_menu;
}

void Player::setMenu(std::shared_ptr<Menu> menu)
{
    m_menu = menu;
}

int Player::getMenuPage() const
{
    return m_menuPage;
}

void Player::setMenuPage(int page)
{
    m_menuPage = page;
}

void Player::connect(std::string_view name,
                     std::string_view ip)
{
    m_name = name;
    m_ip = ip;
    m_connected = true;
    m_userID = GETPLAYERUSERID(m_edict);
}

void Player::disconnect()
{
    m_connected = false;
    m_inGame = false;

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

const char *Player::getName() const
{
    return getNameCore().data();
}

const char *Player::getIPAddress() const
{
    return getIPAddressCore().data();
}

const char *Player::getSteamID() const
{
    return getSteamIDCore().data();
}

edict_t *Player::getEdict() const
{
    return m_edict;
}

unsigned int Player::getIndex() const
{
    return m_index;
}

int Player::getUserId() const
{
    return m_userID;
}

bool Player::isAlive() const
{
    return (m_edict->v.deadflag == DEAD_NO && m_edict->v.health > 0);
}

bool Player::isConnected() const
{
    return m_connected;
}

bool Player::isFake() const
{
    return (m_edict->v.flags & FL_FAKECLIENT || !m_steamID.compare("BOT"));
}

bool Player::isHLTV() const
{
    return (m_edict->v.flags & FL_PROXY || !m_steamID.compare("HLTV"));
}

bool Player::isInGame() const
{
    return m_inGame;
}

IPlayer *PlayerMngr::getPlayer(int index) const
{
    return getPlayerCore(index).get();
}

IPlayer *PlayerMngr::getPlayer(edict_t *edict) const
{
    return getPlayerCore(edict).get();
}

unsigned int PlayerMngr::getMaxClients() const
{
    return m_maxClients;
}

unsigned int PlayerMngr::getNumPlayers() const
{
    return m_playersNum;
}

void PlayerMngr::_initPlayers(edict_t *edictList)
{
    for (size_t i = 1; i <= m_maxClients; i++)
    {
        m_players.at(i) = std::make_shared<Player>(edictList + i, i);
    }
}

std::shared_ptr<Player> PlayerMngr::getPlayerCore(int index) const
{
    try
    {
        return m_players.at(index);
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return nullptr;
    }
}

std::shared_ptr<Player> PlayerMngr::getPlayerCore(edict_t *edict) const
{
    try
    {
        return m_players.at(ENTINDEX(edict));
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return nullptr;
    }
}

void PlayerMngr::_setMaxClients(int maxClients)
{
    m_maxClients = maxClients;
}

bool PlayerMngr::ClientConnect(edict_t *pEntity,
                               const char *pszName,
                               const char *pszAddress,
                               char szRejectReason[128])
{
    using def = ForwardMngr::FwdDefault;

    std::shared_ptr<Forward> fwdPlrConnect = gSPGlobal->getForwardManagerCore()->getDefaultForward(def::ClientConnect);

    int result;
    fwdPlrConnect->pushInt(ENTINDEX(pEntity));
    fwdPlrConnect->pushString(pszName);
    fwdPlrConnect->pushString(pszAddress);
    fwdPlrConnect->execFunc(&result);

    if (static_cast<IForward::ReturnValue>(result) == IForward::ReturnValue::Stop)
        return false;

    return true;
}

void PlayerMngr::ClientConnectPost(edict_t *pEntity,
                                   const char *pszName,
                                   const char *pszAddress)
{
    using def = ForwardMngr::FwdDefault;

    const std::unique_ptr<PlayerMngr> &plrMngr = gSPGlobal->getPlayerManagerCore();
    std::shared_ptr<Player> plr = plrMngr->getPlayerCore(pEntity);
    plr->connect(pszName, pszAddress);

    PlayerMngr::m_playersNum++;

    std::string_view authid(GETPLAYERAUTHID(pEntity));

    if (authid.empty() || !authid.compare("STEAM_ID_PENDING"))
        m_playersToAuth.push_back(plr);
    else
        plr->authorize(authid);

    std::shared_ptr<Forward> fwdPlrConnectPost = gSPGlobal->getForwardManagerCore()->getDefaultForward(def::ClientConnectPost);
    fwdPlrConnectPost->pushInt(ENTINDEX(pEntity));
    fwdPlrConnectPost->pushString(pszName);
    fwdPlrConnectPost->pushString(pszAddress);
    fwdPlrConnectPost->execFunc(nullptr);
}

void PlayerMngr::ClientPutInServer(edict_t *pEntity)
{
    using def = ForwardMngr::FwdDefault;

    std::shared_ptr<Player> plr = getPlayerCore(pEntity);

    std::shared_ptr<Forward> forward = gSPGlobal->getForwardManagerCore()->getDefaultForward(def::ClientPutInServer);
    forward->pushInt(plr->getIndex());
    forward->execFunc(nullptr);
}

void PlayerMngr::ClientPutInServerPost(edict_t *pEntity)
{
    using def = ForwardMngr::FwdDefault;

    std::shared_ptr<Player> plr = getPlayerCore(pEntity);
    plr->putInServer();

    std::shared_ptr<Forward> forward = gSPGlobal->getForwardManagerCore()->getDefaultForward(def::ClientPutInServerPost);
    forward->pushInt(plr->getIndex());
    forward->execFunc(nullptr);
}

void PlayerMngr::ClientUserInfoChangedPost(edict_t *pEntity,
                                           char *infobuffer)
{
    std::shared_ptr<Player> plr = getPlayerCore(pEntity);
    plr->setName(INFOKEY_VALUE(infobuffer, "name"));
}

void PlayerMngr::StartFramePost()
{
    if (m_nextAuthCheck <= gpGlobals->time && !m_playersToAuth.empty())
    {
        m_nextAuthCheck = gpGlobals->time + 0.5f;

        auto iter = m_playersToAuth.begin();
        while (iter != m_playersToAuth.end())
        {
            std::shared_ptr<Player> plr = *iter;
            std::string_view authid(GETPLAYERAUTHID(plr->getEdict()));
            if (!authid.empty() && authid.compare("STEAM_ID_PENDING"))
            {
                plr->authorize(authid);
                iter = m_playersToAuth.erase(iter);
            }
            else
                ++iter;
        }
    }
}

void PlayerMngr::ServerActivatePost(edict_t *pEdictList,
                                    int clientMax)
{
    _setMaxClients(clientMax);
    _initPlayers(pEdictList);
}
