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
            std::stringstream msg;

            msg << '\n';
            msg << "   SPMod - SourcePawn Scripting Engine for Half-Life\n";
            msg << "   Copyright (C) 2018  SPMod Development Team\n";
            msg << '\n';
            msg << "   This program is free software: you can redistribute it and/or modify\n";
            msg << "   it under the terms of the GNU General Public License as published by\n";
            msg << "   the Free Software Foundation, either version 3 of the License, or\n";
            msg << "   (at your option) any later version.\n";
            msg << '\n';
            msg << "   This program is distributed in the hope that it will be useful,\n";
            msg << "   but WITHOUT ANY WARRANTY; without even the implied warranty of\n";
            msg << "   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n";
            msg << "   GNU General Public License for more details.\n";
            msg << '\n';
            msg << "   You should have received a copy of the GNU General Public License\n";
            msg << "   along with this program.  If not, see <https://www.gnu.org/licenses/>.\n";
            msg << '\n';

            SERVER_PRINT(msg.str().c_str());
        }
    }
}
