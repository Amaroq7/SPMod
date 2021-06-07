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

#include "PlayerSystem.hpp"
#include "SPGlobal.hpp"

#include <metamodcpp_sdk/engine/IEdict.hpp>
#include <metamodcpp_sdk/engine/IEntVars.hpp>
#include <metamodcpp_sdk/game/IBasePlayer.hpp>

Player::Player(Metamod::Engine::IEdict *edict)
    : m_basePlayer(gGame->getBasePlayer(edict)), m_edict(edict) {}

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
    Utils::ShowMenu(m_edict, 0, 0, menu);

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
    m_userID = gMetaAPI->getEngine()->getPlayerUserID(m_edict, Metamod::FuncCallType::Direct);
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
    return (
        (m_edict->getEntVars()->getFlags() & Metamod::Engine::EntFlags::FAKECLIENT) == Metamod::Engine::EntFlags::FAKECLIENT
        || m_steamID == "BOT");
}

bool Player::isHLTV() const
{
    return (
        (m_edict->getEntVars()->getFlags() & Metamod::Engine::EntFlags::PROXY) == Metamod::Engine::EntFlags::PROXY
        || m_steamID == "HLTV");
}

bool Player::isInGame() const
{
    return m_inGame;
}

Metamod::Game::IBasePlayer *Player::basePlayer() const
{
    return gGame->getBasePlayer(m_edict);
}

Metamod::Engine::IEdict *Player::edict() const
{
    return m_edict;
}

bool Player::sendMsg(TextMsgDest msgDest, std::string_view message) const
{
    if (!isInGame() || isFake())
    {
        return false;
    }

    Metamod::Game::Mod modType = gGame->getMod();
    bool isCstrikeOrCzero = (modType == Metamod::Game::Mod::CStrike || modType == Metamod::Game::Mod::CZero);
    if (!isCstrikeOrCzero && msgDest == TextMsgDest::Radio)
    {
        return false;
    }

    std::string modifiedMessage(message);
    constexpr auto consoleBytesLimit = 125U;

    if (isCstrikeOrCzero && msgDest == TextMsgDest::Center) // Likely a temporary fix.
    {
        for (auto &character : modifiedMessage)
        {
            if (character == '\n')
            {
                character = '\r';
            }
        }
    }

    if (message.length() > consoleBytesLimit && (msgDest == TextMsgDest::Notify || msgDest == TextMsgDest::Console))
    {
        Utils::trimMultiByteChar(modifiedMessage);
    }
    modifiedMessage.append("\n");

    if (!isCstrikeOrCzero || msgDest == TextMsgDest::Notify || msgDest == TextMsgDest::Console)
    {
        modifiedMessage.append("\n");  // Double newline is required when pre-formatted string in TextMsg is passed as argument.
    }

    Utils::sendTextMsg(modifiedMessage, msgDest, edict());

    return true;
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

Player *PlayerMngr::getPlayer(const Metamod::Engine::IEdict *edict) const
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
        Metamod::Engine::IEdict *spEdict = gMetaAPI->getEngine()->getEdict(i);
        m_players.at(i) = std::make_unique<Player>(spEdict);
    }
}

void PlayerMngr::_setMaxClients(std::uint32_t maxClients)
{
    m_maxClients = maxClients;
}

bool PlayerMngr::ClientConnect(Metamod::Engine::IEdict *pEntity,
                               std::string_view pszName,
                               std::string_view pszAddress,
                               std::string &szRejectReason [[maybe_unused]])
{
    Player *plr = getPlayer(pEntity);
    plr->connect(pszName, pszAddress);

    PlayerMngr::m_playersNum++;

    std::string_view authid(gEngine->getPlayerAuthID(pEntity, Metamod::FuncCallType::Direct));

    if (authid.empty() || authid == "STEAM_ID_PENDING")
        m_playersToAuth.emplace_back(plr);
    else
        plr->authorize(authid);

    return true;
}

void PlayerMngr::ClientPutInServer(Metamod::Engine::IEdict *pEntity)
{
    getPlayer(pEntity)->putInServer();
}

void PlayerMngr::ClientUserInfoChanged(Metamod::Engine::IEdict *pEntity, Metamod::Engine::InfoBuffer infoBuffer)
{
    getPlayer(pEntity)->setName(gEngine->infoKeyValue(infoBuffer, "name", Metamod::FuncCallType::Direct));
}

void PlayerMngr::StartFrame()
{
    if (m_nextAuthCheck <= gEngine->getTime() && !m_playersToAuth.empty())
    {
        m_nextAuthCheck = gEngine->getTime() + 0.5f;

        auto iter = m_playersToAuth.begin();
        while (iter != m_playersToAuth.end())
        {
            Player *plr = *iter;
            std::string_view authid(gEngine->getPlayerAuthID(plr->edict(), Metamod::FuncCallType::Direct));
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

void PlayerMngr::ServerActivate(std::uint32_t clientMax)
{
    _setMaxClients(clientMax);
    _initPlayers();
}

void PlayerMngr::ServerDeactivate()
{
    // Reset player menus
    for (std::size_t i = 1; i <= m_maxClients; i++)
    {
        m_players.at(i)->setMenu(nullptr);
        m_players.at(i)->setMenuPage(0);
    }
}

void PlayerMngr::ClientDrop(Metamod::Engine::IEdict *pEntity,
                            bool crash [[maybe_unused]], std::string_view string [[maybe_unused]])
{
    getPlayer(pEntity)->disconnect();
}
