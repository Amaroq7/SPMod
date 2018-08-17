/*
 *  Copyright (C) 2018 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
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

#if defined _commands_included
    #endinput
#endif
#define _commands_included

typeset CmdCallback
{
    // Server command
    function void (Command cid);

    // Client command
    function PluginReturn (int client, Command cid);
};

methodmap Command
{
    public native Command(const char[] cmd, CmdCallback func, const char[] info = "", bool server = false, const char[] permission = "");
    public native int GetInfo(char[] buffer, int size);
    public native int GetPermission(char[] buffer, int size);
};

forward PluginReturn OnClientCommand(int client);

native int CmdGetArgv(int arg, char[] buffer, int size);
native int CmdGetArgs(char[] buffer, int size);
native int CmdGetArgsNum();