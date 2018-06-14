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

#if defined _messages_included
    #endinput
#endif
#define _messages_included

/**
 * Destination types for MessageBegin()
 */
#define MSG_BROADCAST               0        // Unreliable to all
#define MSG_ONE                     1        // Reliable to one (msg_entity)
#define MSG_ALL                     2        // Reliable to all
#define MSG_INIT                    3        // Write to the init string
#define MSG_PVS                     4        // Ents in PVS of org
#define MSG_PAS                     5        // Ents in PAS of org
#define MSG_PVS_R                   6        // Reliable to PVS
#define MSG_PAS_R                   7        // Reliable to PAS
#define MSG_ONE_UNRELIABLE          8        // Send to one client, but don't put in reliable stream, put in unreliable datagram (could be dropped)
#define MSG_SPEC                    9        // Sends to all spectator proxies

native int GetUserMsgId(const char[] msg_name);

native void GetUserMsgName(int msgid, char[] str, int len);

native void MessageBegin(int dest, int msgid, const int origin[3] = {0,0,0}, int player = 0);

native void MessageBeginF(int dest, int msgid, const float origin[3] = {0.0,0.0,0.0}, int player = 0);

native void MessageEnd();

native void WriteByte(int value);

native void WriteChar(char value);

native void WriteShort(int value);

native void WriteLong(int value);

native void WriteEntity(int value);

native void WriteAngle(int value);

native void WriteAngleF(float value);

native void WriteCoord(int value);

native void WriteCoordF(float value);

native void WriteString(const char[] value);
