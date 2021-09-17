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

#include <ICmdSystem.hpp>
#include <IForwardSystem.hpp>
#include "AnubisInit.hpp"

#include <anubis/observer_ptr.hpp>

using namespace SPMod;

class Player;

/*
 * @brief General command
 */
class Command : public ICommand
{
public:
    Command() = delete;
    ~Command() override = default;
    Command(std::string &&cmd, std::string_view info, ICommand::Callback cb);
    Command(std::regex &&cmd, std::string_view info, ICommand::Callback cb);

    [[nodiscard]] const std::variant<std::string, std::regex> &getNameOrRegex() const override;
    [[nodiscard]] std::string_view getInfo() const override;
    bool execCallback(Player *player);

protected:
    /* command's regex or name */
    std::variant<std::string, std::regex> m_nameOrRegex;

    /* info for cmd, example of usage etc. */
    std::string m_info;

    /* cmd callback */
    ICommand::Callback m_callback;
};

/* @brief Represents client command */
class ClientCommand final : public Command
{
public:
    ClientCommand() = delete;
    ClientCommand(const ClientCommand &other) = default;
    ClientCommand(ClientCommand &&other) = default;
    ~ClientCommand() = default;

    ClientCommand(std::string &&cmd, std::string_view info, std::string_view permission, ICommand::Callback cb);
    ClientCommand(std::regex &&cmd, std::string_view info, std::string_view permission, ICommand::Callback cb);

    bool hasAccess(nstd::observer_ptr<IPlayer> player) const override;
    std::string_view getPermission() const override;

private:
    /* permissions for command */
    std::string m_permission;
};

/* @brief Represents server command */
class ServerCommand final : public Command
{
public:
    ServerCommand() = delete;
    ServerCommand(const ServerCommand &other) = default;
    ServerCommand(ServerCommand &&other) = default;
    ~ServerCommand() = default;

    ServerCommand(std::string_view cmd, std::string_view info, ICommand::Callback cb);

    bool hasAccess(nstd::observer_ptr<IPlayer> player) const final;
    std::string_view getPermission() const final;
};

class CommandMngr final : public ICommandMngr
{
public:
    CommandMngr() = default;
    ~CommandMngr() = default;

    nstd::observer_ptr<ICommand> registerCommand(ICommand::Type type,
                                            std::string_view cmd,
                                            std::string_view info,
                                            bool regex,
                                            std::string_view permission,
                                            ICommand::Callback cb) override;

    template<typename T, typename... Args, typename = std::enable_if_t<std::is_base_of_v<Command, T>>>
    nstd::observer_ptr<Command> registerCommandInternal(Args... args)
    {
        if constexpr (std::is_same_v<::ClientCommand, T>)
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

    bool ClientCommand(nstd::observer_ptr<Anubis::Engine::IEdict> entity, std::string_view cmd);

    static void PluginSrvCommand();
    static void SPModInfoCommand();

private:
    std::vector<std::unique_ptr<Command>> m_clientCommands;
    std::vector<std::unique_ptr<Command>> m_serverCommands;
};
