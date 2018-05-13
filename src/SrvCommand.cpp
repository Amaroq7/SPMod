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
        msg << "plugins - displays currently loaded plugins\n";
        msg << "gpl - displays spmod license";

        logSystem->LogConsoleCore(msg.str());
    }
    else
    {
        std::string arg(CMD_ARGV(1));
        if (arg == "plugins")
        {
            for (auto entry : gSPGlobal->getPluginManagerCore()->getPluginsList())
            {
                logSystem->LogConsoleCore("[", std::setw(3), entry.second->getId(), "] ",
                                        std::setw(-15), entry.second->getName(), " ",
                                        std::setw(-11), entry.second->getVersion(), " ",
                                        std::setw(-11), entry.second->getAuthor(), " ",
                                        std::setw(-11), entry.second->getFilename());
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
    }
}
