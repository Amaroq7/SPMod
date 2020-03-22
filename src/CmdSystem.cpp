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

Command::Command(std::string_view regex, std::string_view info, ICommand::Callback cb, std::any data)
    : m_regex(regex.data()), m_info(info), m_callback(cb), m_data(data)
{
}

const std::regex &Command::getRegex() const
{
    return m_regex;
}

std::string_view Command::getInfo() const
{
    return m_info;
}

IForward::ReturnValue Command::execCallback(Player *player)
{
    return m_callback(player, this, m_data);
}

ClientCommand::ClientCommand(std::string_view cmd,
                             std::string_view info,
                             std::uint32_t flags,
                             ICommand::Callback cb,
                             std::any data)
    : Command(cmd, info, cb, data), m_flags(flags)
{
}

bool ClientCommand::hasAccess(const IPlayer *player [[maybe_unused]]) const
{
    return _hasAccess(/*player->getFlags()*/ 0);
}

uint32_t ClientCommand::getAccess() const
{
    return m_flags;
}

bool ClientCommand::_hasAccess(uint32_t flags [[maybe_unused]]) const
{
    // TODO: Check access
    return true;
}

ServerCommand::ServerCommand(std::string_view cmd, std::string_view info, ICommand::Callback cb, std::any data)
    : Command(cmd, info, cb, data)
{
}

bool ServerCommand::hasAccess(const IPlayer *player [[maybe_unused]]) const
{
    return true;
}

std::uint32_t ServerCommand::getAccess() const
{
    return 0;
}

Command *CommandMngr::registerCommand(ICommand::Type type,
                                      std::string_view cmd,
                                      std::string_view info,
                                      std::uint32_t flags,
                                      ICommand::Callback cb,
                                      std::any data)
{
    switch (type)
    {
        case ICommand::Type::Client:
            return registerCommandInternal<ClientCommand>(cmd, info, flags, cb, data).get();
        case ICommand::Type::Server:
            return registerCommandInternal<ServerCommand>(cmd, info, cb, data).get();
    }
}

std::size_t CommandMngr::getCommandsNum(ICommand::Type type)
{
    return (type == ICommand::Type::Client ? m_clientCommands.size() : m_serverCommands.size());
}

void CommandMngr::clearCommands()
{
    m_clientCommands.clear();
    m_serverCommands.clear();
}

META_RES CommandMngr::ClientCommandMeta(edict_t *entity, std::string_view clCmd)
{
    Player *player = gSPGlobal->getPlayerManager()->getPlayer(entity);
    META_RES metaResult = MRES_IGNORED;
    std::string cmdName(clCmd);
    if (getCommandsNum(ICommand::Type::Client))
    {
        if (clCmd == "say" || clCmd == "say_team")
        {
            cmdName += ' ';
            cmdName += CMD_ARGV(1);
        }

        for (const auto &cmd : getCommandList(ICommand::Type::Client))
        {
            if (std::regex_search(cmdName, cmd->getRegex()) && cmd->hasAccess(player))
            {
                IForward::ReturnValue result = cmd->execCallback(player);
                if (result == IForward::ReturnValue::Stop || result == IForward::ReturnValue::Handled)
                {
                    metaResult = MRES_SUPERCEDE;
                    if (result == IForward::ReturnValue::Stop)
                        break;
                }
            }
        }
    }
    return metaResult;
}