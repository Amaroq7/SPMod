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

class Player;

/*
 * @brief General command
 */
class Command : public ICommand
{
public:
    Command() = delete;
    virtual ~Command() = default;
    Command(std::string_view cmd, std::string_view info, ICommand::Callback cb, std::any data);

    const std::regex &getRegex() const override;
    std::string_view getInfo() const override;
    IForward::ReturnValue execCallback(Player *player);

protected:
    /* command's regex */
    std::regex m_regex;

    /* info for cmd, example of usage etc. */
    std::string m_info;

    /* cmd callback */
    ICommand::Callback m_callback;

    /* callback data */
    std::any m_data;
};

/* @brief Represents client command */
class ClientCommand final : public Command
{
public:
    ClientCommand() = delete;
    ClientCommand(const ClientCommand &other) = default;
    ClientCommand(ClientCommand &&other) = default;
    ~ClientCommand() = default;

    ClientCommand(std::string_view cmd,
                  std::string_view info,
                  std::uint32_t flags,
                  ICommand::Callback cb,
                  std::any data);

    bool hasAccess(const IPlayer *player) const override;
    uint32_t getAccess() const override;

private:
    bool _hasAccess(uint32_t flags) const;

private:
    /* permissions for command */
    uint32_t m_flags;
};

/* @brief Represents server command */
class ServerCommand final : public Command
{
public:
    ServerCommand() = delete;
    ServerCommand(const ServerCommand &other) = default;
    ServerCommand(ServerCommand &&other) = default;
    ~ServerCommand() = default;

    ServerCommand(std::string_view cmd, std::string_view info, ICommand::Callback cb, std::any data);

    bool hasAccess(const IPlayer *player) const override;
    std::uint32_t getAccess() const override;
};

class CommandMngr final : public ICommandMngr
{
public:
    CommandMngr() = default;
    ~CommandMngr() = default;

    Command *registerCommand(ICommand::Type type,
                             std::string_view cmd,
                             std::string_view info,
                             std::uint32_t flags,
                             ICommand::Callback cb,
                             std::any data);

    template<typename T, typename... Args, typename = std::enable_if_t<std::is_base_of_v<Command, T>>>
    const std::unique_ptr<Command> &registerCommandInternal(Args... args)
    {
        if constexpr (std::is_same_v<ClientCommand, T>)
        {
            return m_clientCommands.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        }
        else
        {
            return m_serverCommands.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        }
    }

    const auto &getCommandList(ICommand::Type type) const
    {
        return (type == ICommand::Type::Client ? m_clientCommands : m_serverCommands);
    }

    std::size_t getCommandsNum(ICommand::Type type);
    void clearCommands();

    META_RES ClientCommandMeta(edict_t *entity, std::string_view cmd);

private:
    std::vector<std::unique_ptr<Command>> m_clientCommands;
    std::vector<std::unique_ptr<Command>> m_serverCommands;
};
