/*  SPMod - SourcePawn Scripting Engine for Half-Life
 *  Copyright (C) 2018  SPMod Development Team
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

Command::Command(std::string_view cmd,
                 std::string_view info) : m_cmd(cmd),
                                          m_info(info)
{
}

const char *Command::getCmd() const
{
    return getCmdCore().data();
}

const char *Command::getInfo() const
{
    return getInfoCore().data();
}

std::string_view Command::getCmdCore() const
{
    return m_cmd;
}

std::string_view Command::getInfoCore() const
{
    return m_info;
}

ClientCommand::ClientCommand(std::string_view cmd,
                             std::string_view info,
                             std::uint32_t flags) : Command(cmd, info),
                                                    m_flags(flags)
{
}

bool ClientCommand::hasAccess(IPlayer *player) const
{
    return hasAccessCore(gSPGlobal->getPlayerManagerCore()->getPlayerCore(player->getIndex()));
}

bool ClientCommand::hasAccessCore(std::shared_ptr<Player> player) const
{
    // TODO: Check access
    return true;
}

uint32_t ClientCommand::getAccess() const
{
    return m_flags;
}

ServerCommand::ServerCommand(std::string_view cmd,
                             std::string_view info) : Command(cmd, info)
{
}

bool ServerCommand::hasAccess(IPlayer *player [[maybe_unused]]) const
{
    return hasAccessCore(nullptr);
}

bool ServerCommand::hasAccessCore(std::shared_ptr<Player> player [[maybe_unused]]) const
{
    return true;
}

std::uint32_t ServerCommand::getAccess() const
{
    return 0;
}

ICommand *CommandMngr::registerCommand(ICommand::Type type, const char *cmd, const char *info, std::uint32_t flags)
{
    switch (type)
    {
        case ICommand::Type::Client: return registerCommandCore<ClientCommand>(cmd, info, flags).get();
        case ICommand::Type::Server: return registerCommandCore<ServerCommand>(cmd, info).get();
        default: return nullptr;
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
