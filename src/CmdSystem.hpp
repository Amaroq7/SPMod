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

#pragma once

#include "spmod.hpp"

class Player;

/*
 * @brief General command
 */
class Command : public ICommand
{
public:
    Command() = delete;
    ~Command() = default;
    Command(std::string_view cmd,
            std::string_view info);

    const char *getCmd() const override;
    const char *getInfo() const override;

    std::string_view getCmdCore() const;
    std::string_view getInfoCore() const;

    virtual bool hasAccessCore(std::shared_ptr<Player> player) const = 0;

protected:
    /* name of command or regex */
    std::string m_cmd;

    /* info for cmd, example of usage etc. */
    std::string m_info;
};

/* @brief Represents client command */
class ClientCommand final : public Command
{
public:
    ClientCommand() = delete;
    ClientCommand(const ClientCommand &other) = delete;
    ClientCommand(ClientCommand &&other) = default;
    ~ClientCommand() = default;

    ClientCommand(std::string_view cmd,
                  std::string_view info,
                  std::uint32_t flags);

    bool hasAccess(IPlayer *player) const override;
    bool hasAccessCore(std::shared_ptr<Player> player) const override;
    uint32_t getAccess() const override;

private:
    /* permissions for command */
    uint32_t m_flags;
};

/* @brief Represents server command */
class ServerCommand final : public Command
{
public:
    ServerCommand() = delete;
    ServerCommand(const ServerCommand &other) = delete;
    ServerCommand(ServerCommand &&other) = default;
    ~ServerCommand() = default;

    ServerCommand(std::string_view cmd,
                  std::string_view info);

    bool hasAccess(IPlayer *player) const override;
    bool hasAccessCore(std::shared_ptr<Player> player) const override;
    std::uint32_t getAccess() const override;
};

class CommandMngr final : public ICommandMngr
{
public:
    CommandMngr() = default;
    ~CommandMngr() = default;

    ICommand *registerCommand(ICommand::Type type, const char *cmd, const char *info, std::uint32_t flags) override;

    template<typename T, typename ...Args, typename = std::enable_if_t<std::is_base_of_v<Command, T>>>
    std::shared_ptr<Command> registerCommandCore(Args... args)
    {
        auto cmd = std::make_shared<T>(std::forward<Args>(args)...);

        if constexpr (std::is_same_v<ClientCommand, T>)
            return m_clientCommands.emplace_back(cmd);
        else
            return m_serverCommands.emplace_back(cmd);
    }

    const auto &getCommandList(ICommand::Type type) const
    {
        return (type == ICommand::Type::Client ? m_clientCommands : m_serverCommands);
    }

    std::size_t getCommandsNum(ICommand::Type type);
    void clearCommands();

private:
    std::vector<std::shared_ptr<Command>> m_clientCommands;
    std::vector<std::shared_ptr<Command>> m_serverCommands;
};
