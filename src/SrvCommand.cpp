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
#include "CompilationUtils.hpp"

static constexpr std::size_t nameWidth = 25;
static constexpr std::size_t verWidth = 15;
static constexpr std::size_t authWidth = 20;
static constexpr std::size_t fileWidth = 15;

void SPModInfoCommand()
{
    auto logger = gSPGlobal->getLoggerManager()->getLogger(gSPModLoggerName);
    // Print out available commands
    if (CMD_ARGC() == 1)
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
        std::string_view arg(CMD_ARGV(1));

        if (arg == "plugins")
        {
            logger->sendMsgToConsoleInternal(std::left, std::setw(7), "\n", std::setw(nameWidth), "name",
                                             std::setw(verWidth), "version", std::setw(authWidth), "author",
                                             "filename");
            std::size_t pos = 1;

            for (const auto interface : gSPGlobal->getAdaptersInterfaces())
            {
                for (const auto plugin : interface.second->getPluginMngr()->getPluginsList())
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

void PluginSrvCmd()
{
    std::string_view argv(CMD_ARGV(0));

    for (const auto &cmd : gSPGlobal->getCommandManager()->getCommandList(Command::Type::Server))
    {
        if (std::regex_search(argv.data(), cmd->getRegex()))
        {
            cmd->execCallback(nullptr);
        }
    }
}