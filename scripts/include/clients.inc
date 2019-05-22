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

#if defined _clients_included
    #endinput
#endif
#define _clients_included

methodmap Player
{
    public native int GetName(char[] buffer, int size);
    public native int GetIP(char[] buffer, int size, bool port = false);
    public native int GetSteamID(char[] buffer, int size);

    public native bool HasAccess(const char[] permission);
    public native void AttachGroup(Group group);
    public native void RemoveGroup(Group group);
    public native void AttachPermission(const char[] permission);
    public native void RemovePermission(const char[] permission);

    property int Index {
        public native get();
    }
    property int UserID {
        public native get();
    }
    property bool Alive {
        public native get();
    }
    property bool Connected {
        public native get();
    }
    property bool Fake {
        public native get();
    }
    property bool HLTV {
        public native get();
    }
    property bool InGame {
        public native get();
    }
}

/*
 * @brief Called when a client connects to the server.
 *
 * @param client        Client id.
 * @param name          Name of the client.
 * @param ip            IP address of the client.
 * @param reason        Reason that will be displayed to the client when they get rejected.
 *
 * @return              PluginContinue to let client join or PluginStop to reject them.
 */
forward PluginReturn OnClientConnect(Player client, const char[] name, const char[] ip, char reason[128]);

/*
 * @brief Called when a client disconnects from the server.
 *
 * @param client        Client id.
 * @param crash         True if client crashed, false otherwise.
 * @param reason        Reason why the client has been disconnected.
 *
 * @noreturn
 */
forward void OnClientDisconnect(Player client, bool crash, const char[] reason);

/*
 * @brief Called when a client enters the game.
 *
 * @param client        Client id.
 *
 * @noreturn
 */
forward void OnClientPutInServer(Player client);

/*
 * @brief Called when a client permissions changed.
 *
 * @param client        Client id.
 *
 * @noreturn
 */
forward void OnClientPermissionsChanged(Player client);
