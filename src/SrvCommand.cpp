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
    // Print out available commands
    if (CMD_ARGC() == 1)
    {
        std::stringstream msg;

        msg << '\n';
        msg << "Usage: spmod [command] [args]" << '\n';
        msg << "Command:" << '\n';
        msg << "plugins - displays currently loaded plugins" << '\n';
        msg << "gpl - displays spmod license" << '\n';

        SERVER_PRINT(msg.str().c_str());
    }
    else
    {
        std::string arg(CMD_ARGV(1));
        if (arg == "plugins")
        {
            for (auto entry : gSPGlobal->getPluginManagerCore()->getPluginsList())
            {
                LOG_CONSOLE(PLID, "[%3d] %-15.15s %-11.10s %-11.20s %-11.20s\n",
                                    entry.second->getId(),
                                    entry.second->getName(),
                                    entry.second->getVersion(),
                                    entry.second->getAuthor(),
                                    entry.second->getFileName());
            }
        }
        else if (arg == "gpl")
        {
            auto &logSystem = gSPGlobal->getLoggerCore();
            
            logSystem->LogConsoleCore(""); //Empty line
            logSystem->LogConsoleCore("   SPMod - SourcePawn Scripting Engine for Half-Life");
            logSystem->LogConsoleCore("   Copyright (C) 2018  SPMod Development Team");
            logSystem->LogConsoleCore("");
            logSystem->LogConsoleCore("   This program is free software: you can redistribute it and/or modify");
            logSystem->LogConsoleCore("   it under the terms of the GNU General Public License as published by");
            logSystem->LogConsoleCore("   the Free Software Foundation, either version 3 of the License, or");
            logSystem->LogConsoleCore("   (at your option) any later version.");
            logSystem->LogConsoleCore("");
            logSystem->LogConsoleCore("   This program is distributed in the hope that it will be useful,");
            logSystem->LogConsoleCore("   but WITHOUT ANY WARRANTY; without even the implied warranty of");
            logSystem->LogConsoleCore("   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the");
            logSystem->LogConsoleCore("   GNU General Public License for more details.");
            logSystem->LogConsoleCore("");
            logSystem->LogConsoleCore("   You should have received a copy of the GNU General Public License");
            logSystem->LogConsoleCore("   along with this program.  If not, see <https://www.gnu.org/licenses/>.");
            logSystem->LogConsoleCore("");
        }
    }
}
