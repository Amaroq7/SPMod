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

std::string_view Command::getCmd() const
{
    return m_cmd;
}

std::string_view Command::getInfo() const
{
    return m_info;
}

std::size_t Command::getId() const
{
    return m_id;
}

SourcePawn::IPluginFunction *Command::getFunc() const
{
    return m_func;
}

ClientCommand::ClientCommand(std::size_t id,
                             std::string_view cmd,
                             std::string_view info,
                             SourcePawn::IPluginFunction *func,
                             uint32_t flags) : m_flags(flags)
{
    m_id = id;
    m_cmd = cmd;
    m_info = info;
    m_func = func;
}

bool ClientCommand::hasAccess(/*SPPlayer *player*/) const
{
    // TODO: Check access
    return true;
}

uint32_t ClientCommand::getAccess() const
{
    return m_flags;
}

ServerCommand::ServerCommand(std::size_t id,
                             std::string_view cmd,
                             std::string_view info,
                             SourcePawn::IPluginFunction *func)
{
    m_id = id;
    m_cmd = cmd;
    m_info = info;
    m_func = func;
}

bool ServerCommand::hasAccess(/*SPPlayer *player [[maybe_unused]]*/) const
{
    return true;
}

uint32_t ServerCommand::getAccess() const
{
    return 0;
}

std::shared_ptr<Command> CommandMngr::getCommand(std::size_t id)
{
    for (auto cmd : m_clientCommands)
    {
        if (cmd->getId() == id)
            return cmd;
    }

    for (auto cmd : m_serverCommands)
    {
        if (cmd->getId() == id)
            return cmd;
    }

    return nullptr;
}

std::size_t CommandMngr::getCommandsNum(CmdType type)
{
    return (type == CmdType::Client ? m_clientCommands.size() : m_serverCommands.size());
}

void CommandMngr::clearCommands()
{
    m_clientCommands.clear();
    m_serverCommands.clear();
    m_cid = 0;
}
