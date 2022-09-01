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

#include "CmdSystem.hpp"
#include "SPGlobal.hpp"

#include <SPConfig.hpp>
#include <utility>

Command::Command(std::string &&cmd, std::string_view info, ICommand::Callback cb)
    : m_nameOrRegex(std::move(cmd)), m_info(info), m_callback(std::move(cb))
{
}

Command::Command(std::regex &&cmd, std::string_view info, ICommand::Callback cb)
    : m_nameOrRegex(std::move(cmd)), m_info(info), m_callback(std::move(cb))
{
}

const std::variant<std::string, std::regex> &Command::getNameOrRegex() const
{
    return m_nameOrRegex;
}

std::string_view Command::getInfo() const
{
    return m_info;
}

bool Command::execCallback(Player *player)
{
    return m_callback(player, this);
}

ClientCommand::ClientCommand(std::string &&cmd,
                             std::string_view info,
                             std::string_view permission,
                             ICommand::Callback cb)
    : Command(std::move(cmd), info, std::move(cb)), m_permission(permission)
{
}

ClientCommand::ClientCommand(std::regex &&cmd,
                             std::string_view info,
                             std::string_view permission,
                             ICommand::Callback cb)
    : Command(std::move(cmd), info, std::move(cb)), m_permission(permission)
{
}

bool ClientCommand::hasAccess(nstd::observer_ptr<IPlayer> player) const
{
    return player->hasAccess(m_permission);
}

std::string_view ClientCommand::getPermission() const
{
    return m_permission;
}

ServerCommand::ServerCommand(std::string_view cmd, std::string_view info, ICommand::Callback cb)
    : Command(std::string(cmd), info, std::move(cb))
{
}

bool ServerCommand::hasAccess(nstd::observer_ptr<IPlayer> player [[maybe_unused]]) const
{
    return true;
}

std::string_view ServerCommand::getPermission() const
{
    return {};
}

nstd::observer_ptr<ICommand> CommandMngr::registerCommand(ICommand::Type type,
                                                     std::string_view cmd,
                                                     std::string_view info,
                                                     bool regex,
                                                     std::string_view permission,
                                                     ICommand::Callback cb)
{
    switch (type)
    {
        case ICommand::Type::Client:
        {
            if (regex)
            {
                return registerCommandInternal<::ClientCommand>(std::regex(cmd.data()), info, permission, cb);
            }
            return registerCommandInternal<::ClientCommand>(std::string(cmd), info, permission, cb);
        }
        case ICommand::Type::Server:
        {
            if (regex)
            {
                return {}; // Server command can't be a regex expression
            }
            gEngine->registerSrvCommand(cmd.data(), CommandMngr::PluginSrvCommand, Anubis::FuncCallType::Direct);
            return registerCommandInternal<ServerCommand>(cmd, info, cb);
        }
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

bool CommandMngr::ClientCommand(nstd::observer_ptr<Anubis::Engine::IEdict> entity, std::string_view clCmd)
{
    Player *player = gSPGlobal->getPlayerManager()->getPlayer(entity);
    bool result = true;
    if (getCommandsNum(ICommand::Type::Client))
    {
        std::string cmdName(clCmd);
        if (clCmd == "say" || clCmd == "say_team")
        {
            cmdName += ' ';
            cmdName += gEngine->cmdArgv(1, Anubis::FuncCallType::Direct);
        }

        for (const auto &cmd : getCommandList(ICommand::Type::Client))
        {
            bool commandMatched = std::visit(
                [&cmdName](auto &&arg) {
                    using T = std::decay_t<decltype(arg)>;
                    if constexpr (std::is_same_v<T, std::string>)
                    {
                        return cmdName == arg;
                    }
                    else if constexpr (std::is_same_v<T, std::regex>)
                    {
                        return std::regex_search(cmdName, arg);
                    }

                    return false;
                }, cmd->getNameOrRegex());
            if (commandMatched && cmd->hasAccess(player))
            {
                result = cmd->execCallback(player);
            }
        }
    }
    return result;
}

void CommandMngr::PluginSrvCommand()
{
    std::string_view argv(gEngine->cmdArgv(0, Anubis::FuncCallType::Direct));

    for (const auto &cmd : gSPGlobal->getCommandManager()->getCommandList(Command::Type::Server))
    {
        if (argv == std::get<std::string>(cmd->getNameOrRegex()))
        {
            cmd->execCallback(nullptr);
        }
    }
}

void CommandMngr::SPModInfoCommand()
{
    static constexpr std::size_t nameWidth = 25;
    static constexpr std::size_t verWidth = 15;
    static constexpr std::size_t authWidth = 20;
    static constexpr std::size_t fileWidth = 15;

    auto logger = gSPGlobal->getLoggerManager()->getLogger(gSPModLoggerName);
    // Print out available commands
    if (gEngine->cmdArgc(Anubis::FuncCallType::Direct) == 1)
    {
        logger->sendMsgToConsoleInternal("\nUsage: spmod [command] [args]\n \
                                      Command:\n \
                                      version - displays currently version\n \
                                      plugins - displays currently loaded plugins\n \
                                      adapters - displays currently loaded adapters\n \
                                      gpl - displays spmod license");
    }
    else
    {
        std::string_view arg(gEngine->cmdArgv(1, Anubis::FuncCallType::Direct));

        if (arg == "plugins")
        {
            logger->sendMsgToConsoleInternal(std::left, std::setw(7), "\n", std::setw(nameWidth), "name",
                                             std::setw(verWidth), "version", std::setw(authWidth), "author",
                                             "filename");
            std::size_t pos = 1;

            for (const auto &[key, interface] : gSPGlobal->getAdaptersInterfaces())
            {
                for (const auto plugin : interface->getPluginMngr()->getPluginsList())
                {
                    logger->sendMsgToConsoleInternal("[", std::right, std::setw(3), pos++,
                                                     "] ",                 // right align for ordinal number
                                                     std::left,            // left align for the rest
                                                     std::setw(nameWidth), // format rules for name
                                                     std::string_view(plugin->getName()).substr(0, nameWidth - 1),
                                                     std::setw(verWidth), // format rules for version
                                                     std::string_view(plugin->getVersion()).substr(0, verWidth - 1),
                                                     std::setw(authWidth), // format rules for author
                                                     std::string_view(plugin->getAuthor()).substr(0, authWidth - 1),
                                                     std::string_view(plugin->getFilename()).substr(0, fileWidth));
                }
            }
        }
        else if (arg == "gpl")
        {
            logger->sendMsgToConsoleInternal("   SPMod - SourcePawn Scripting Engine for Half-Life\n \
  Copyright (C) 2018-",
                                             gCompilationYear, " SPMod Development Team\n\n \
  \
SPMod is free software: you can redistribute it and/or modify\n \
  it under the terms of the GNU General Public License as published by\n \
  the Free Software Foundation, either version 3 of the License, or\n \
  (at your option) any later version.\n\n \
  \
SPMod is distributed in the hope that it will be useful,\n \
  but WITHOUT ANY WARRANTY; without even the implied warranty of\n \
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n \
  GNU General Public License for more details.\n\n \
  \
You should have received a copy of the GNU General Public License\n \
  along with SPMod.  If not, see <https://www.gnu.org/licenses/>.");
        }
        if (arg == "adapters")
        {
            logger->sendMsgToConsoleInternal(std::left, std::setw(7), '\n', std::setw(nameWidth), "name",
                                             std::setw(verWidth), "version", std::setw(authWidth), "author");
            std::size_t pos = 1;
            for (const auto &entry : gSPGlobal->getAdaptersInterfaces())
            {
                logger->sendMsgToConsoleInternal("[", std::right, std::setw(3), pos++,
                                                 "] ",                 // right align for ordinal number
                                                 std::left,            // left align for the rest
                                                 std::setw(nameWidth), // format rules for name
                                                 entry.second->getExtName(),
                                                 std::setw(verWidth), // format rules for version
                                                 entry.second->getVersion(),
                                                 std::setw(authWidth), // format rules for author
                                                 entry.second->getAuthor());
            }
        }
        else if (arg == "version")
        {
            logger->sendMsgToConsoleInternal(CNSL_LBLUE, "SPMod ", CNSL_RESET, CNSL_LGREEN, "v", gSPModVersion);
            logger->sendMsgToConsoleInternal(CNSL_LBLUE, "SPMod API: ", CNSL_RESET, CNSL_LGREEN, "v",
                                             ISPGlobal::MAJOR_VERSION, ".", ISPGlobal::MINOR_VERSION);
            logger->sendMsgToConsoleInternal(CNSL_LBLUE, "SPMod build: ", CNSL_RESET, CNSL_LGREEN, gCompilationTime,
                                             " ", gCompilationDate);
            logger->sendMsgToConsoleInternal(CNSL_LBLUE, "SPMod from: ", CNSL_RESET, CNSL_LGREEN, APP_COMMIT_URL,
                                             APP_COMMIT_SHA);
        }
    }
}