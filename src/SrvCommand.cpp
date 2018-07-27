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

void SPModInfoCommand()
{
    auto &logSystem = gSPGlobal->getLoggerCore();

    // Print out available commands
    if (CMD_ARGC() == 1)
    {
        std::stringstream msg;

        msg << "\nUsage: spmod [command] [args]\n";
        msg << "Command:\n";
        msg << "version - displays currently version\n";
        msg << "plugins - displays currently loaded plugins\n";
        msg << "gpl - displays spmod license";

        logSystem->LogConsoleCore(msg.str());
    }
    else
    {
        std::string arg(CMD_ARGV(1));

        static constexpr std::size_t nameWidth = 25;
        static constexpr std::size_t verWidth = 15;
        static constexpr std::size_t authWidth = 20;
        static constexpr std::size_t fileWidth = 15;
        
        if (arg == "plugins")
        {
            logSystem->LogConsoleCore(std::left,
                                      std::setw(7),
                                      "\n",
                                      std::setw(nameWidth),
                                      "name",
                                      std::setw(verWidth),
                                      "version",
                                       std::setw(authWidth),
                                      "author",
                                      "filename");
            std::size_t pos = 1;
            for (auto entry : gSPGlobal->getPluginManagerCore()->getPluginsList())
            {
                logSystem->LogConsoleCore("[", std::right, std::setw(3), pos++, "] ", // right align for ordinal number
                                        std::left, // left align for the rest
                                        std::setw(nameWidth), // format rules for name
                                        entry.second->getNameCore().substr(0, nameWidth - 1),
                                        std::setw(verWidth), // format rules for version
                                        entry.second->getVersionCore().substr(0, verWidth - 1),
                                        std::setw(authWidth), // format rules for author
                                        entry.second->getAuthorCore().substr(0, authWidth - 1),
                                        entry.second->getFileNameCore().substr(0, fileWidth));
            }
        }
        else if (arg == "gpl")
        {
            logSystem->LogConsoleCore("   SPMod - SourcePawn Scripting Engine for Half-Life\n \
  Copyright (C) 2018  SPMod Development Team\n\n \
  \
This program is free software: you can redistribute it and/or modify\n \
  it under the terms of the GNU General Public License as published by\n \
  the Free Software Foundation, either version 3 of the License, or\n \
  (at your option) any later version.\n\n \
  \
This program is distributed in the hope that it will be useful,\n \
  but WITHOUT ANY WARRANTY; without even the implied warranty of\n \
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n \
  GNU General Public License for more details.\n\n \
  \
You should have received a copy of the GNU General Public License\n \
  along with this program.  If not, see <https://www.gnu.org/licenses/>.");
        }
        else if(arg == "version")
        {
            logSystem->LogConsoleCore("SPMod ", gSPModVersion, ", API ", SPMOD_API_VERSION, \
                "\nSPMod build: ", __TIME__, " ", __DATE__, \
                "\nSPMod from: ", APP_COMMIT_URL, APP_COMMIT_SHA);
        }
    }
}

void PluginSrvCmd()
{
    const char *argv = CMD_ARGV(0);

    for (const auto &cmd : gSPGlobal->getCommandManagerCore()->getCommandList(CmdType::Server))
    {
        if (!cmd->getCmd().compare(argv))
        {
            cell_t result;
            SourcePawn::IPluginFunction *func = cmd->getFunc();
            func->PushCell(cmd->getId());
            func->Execute(&result);
            break;
        }
    }
}