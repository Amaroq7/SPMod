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

/*
 * @brief General command 
 */
class Command
{
public:
    virtual ~Command() {};

    std::string_view getCmd() const;
    std::string_view getInfo() const;
    std::size_t getId() const;
    SourcePawn::IPluginFunction *getFunc() const;

    virtual bool hasAccess(/*SPPlayer *player = nullptr*/) const = 0;
    virtual uint32_t getAccess() const = 0;

protected:
    /* command id */
    std::size_t m_id;

    /* name of command or regex */
    std::string m_cmd;

    /* info for cmd, example of usage etc. */
    std::string m_info;

    /* func to be executed in plugin */
    SourcePawn::IPluginFunction *m_func;
};

/* @brief Represents client command */
class ClientCommand final : public Command
{
public:
    ClientCommand() = delete;
    ~ClientCommand() = default;

    ClientCommand(std::size_t id,
                  std::string_view cmd,
                  std::string_view info,
                  SourcePawn::IPluginFunction *func,
                  uint32_t flags);

    bool hasAccess(/*SPPlayer *player*/) const override;
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
    ~ServerCommand() = default;

    ServerCommand(std::size_t id,
                  std::string_view cmd,
                  std::string_view info,
                  SourcePawn::IPluginFunction *func);

    bool hasAccess(/*SPPlayer *player [[maybe_unused]]*/) const override;
    uint32_t getAccess() const override;
};

enum class CmdType : uint8_t
{
    Client = 0,
    Server = 1
};

class CommandMngr final
{
public:
    CommandMngr() = default;
    ~CommandMngr() = default;

    template<typename T, typename ...Args, typename = std::enable_if_t<std::is_base_of_v<Command, T>>>
    std::shared_ptr<Command> registerCommand(Args... args)
    {
        auto cmd = std::make_shared<T>(m_cid++, std::forward<Args>(args)...);

        if constexpr (std::is_same_v<ClientCommand, T>)
            return m_clientCommands.emplace_back(cmd);
        else
            return m_serverCommands.emplace_back(cmd);
    }
    
    const auto &getCommandList(CmdType type) const
    {
        return (type == CmdType::Client ? m_clientCommands : m_serverCommands);
    }

    std::shared_ptr<Command> getCommand(std::size_t id);
    std::size_t getCommandsNum(CmdType type);
    void clearCommands();

private:
    /* keeps track of command ids */
    std::size_t m_cid;

    std::vector<std::shared_ptr<Command>> m_clientCommands;
    std::vector<std::shared_ptr<Command>> m_serverCommands;
};