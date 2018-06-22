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

    std::string_view getCmd() const
    {
        return m_cmd;
    }

    std::string_view getInfo() const
    {
        return m_info;
    }

    size_t getId() const
    {
        return m_id;
    }

    SourcePawn::IPluginFunction *getFunc() const
    {
        return m_func;
    }

    virtual bool hasAccess(/*SPPlayer *player = nullptr*/) const = 0;
    virtual uint32_t getAccess() const = 0;

protected:
    /* command id */
    size_t m_id;

    /* name of command or regex */
    std::string m_cmd;

    /* info for cmd, example of usage etc. */
    std::string m_info;

    /* func to be executed in plugin */
    SourcePawn::IPluginFunction *m_func;
};

/* @brief Represents client command */
class ClientCommand : public Command
{
public:
    ClientCommand() = delete;
    ~ClientCommand() = default;

    ClientCommand(size_t id,
                  std::string_view cmd,
                  std::string_view info,
                  SourcePawn::IPluginFunction *func,
                  uint32_t flags);

    bool hasAccess(/*SPPlayer *player*/) const override
    {
        // TODO: Check access
        return true;
    }

    uint32_t getAccess() const override
    {
        return m_flags;
    }

private:
    /* permissions for command */
    uint32_t m_flags;
};

/* @brief Represents server command */
class ServerCommand : public Command
{
public:
    ServerCommand() = delete;
    ~ServerCommand() = default;

    ServerCommand(size_t id,
                  std::string_view cmd,
                  std::string_view info,
                  SourcePawn::IPluginFunction *func);

    bool hasAccess(/*SPPlayer *player [[maybe_unused]]*/) const override
    {
        return true;
    }

    uint32_t getAccess() const override
    {
        return 0;
    }
};

enum class CmdType : uint8_t
{
    Client = 0,
    Server = 1
};

class CommandMngr
{
public:
    CommandMngr() = default;
    ~CommandMngr() = default;

    template<typename T, typename ...Args, typename = std::enable_if_t<std::is_base_of_v<Command, T>>>
    std::shared_ptr<Command> registerCommand(Args... args)
    {
        std::shared_ptr<T> cmd = std::make_shared<T>(m_cid++, std::forward<Args>(args)...);

        if constexpr (std::is_same_v<ClientCommand, T>)
            m_clientCommands.push_back(cmd);
        else
            m_serverCommands.push_back(cmd);

        return cmd;
    }
    
    const auto &getCommandList(CmdType type) const
    {
        return (type == CmdType::Client ? m_clientCommands : m_serverCommands);
    }
    
    std::shared_ptr<Command> getCommand(size_t id)
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

    size_t getCommandsNum(CmdType type)
    {
        return (type == CmdType::Client ? m_clientCommands.size() : m_serverCommands.size());
    }

    void clearCommands()
    {
        m_clientCommands.clear();
        m_serverCommands.clear();
        m_cid = 0;
    }

private:
    /* keeps track of command ids */
    size_t m_cid;

    std::vector<std::shared_ptr<Command>> m_clientCommands;
    std::vector<std::shared_ptr<Command>> m_serverCommands;
};