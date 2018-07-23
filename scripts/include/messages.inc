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

/**
 * @brief Returns the ID of a given message, or -1 on failure.
 *
 * @param msg_name     String containing message name (case sensitive).
 *
 * @return             A message index, or -1 on failure.
 */
native int GetUserMsgId(const char[] msg_name);

/**
 * @brief Retrieves the client message name from a message id.
 *
 * @param msgid        Message index.
 * @param str          Buffer to store the name of the message.
 * @param len          Maximum length of string buffer.
 *
 * @return             String len or Zero if msgid is invalid.
 */
native int GetUserMsgName(int msgid, char[] str, int len);

/**
 * @brief Starts a message.
 *
 * @param dest         Destination type.
 * @param msgid        Message index.
 * @param origin       Message origin.
 * @param player       Player index to broadcast to.
 *
 * @noreturn
 * @error              Invalid message index, invalid number of parameters passed.
 */
native void MessageBegin(int dest, int msgid, int origin[3] = {0,0,0}, int player = 0);

/**
 * @brief Starts a message.
 *
 * @param dest         Destination type.
 * @param msgid        Message index.
 * @param origin       Message origin.
 * @param player       Player index to broadcast to.
 *
 * @noreturn
 * @error              Invalid message index, invalid number of parameters passed.
 */
native void MessageBeginF(int dest, int msgid, float origin[3] = {0.0,0.0,0.0}, int player = 0);

/**
 * @brief Ends a message.
 *
 * @noreturn
 */
native void MessageEnd();

/**
 * @brief Writes a byte to a message buffer.
 *
 * @note Use after MessageBegin().
 *
 * @param value        Byte to write (value will be written as 8bit).
 *
 * @noreturn
 */
native void WriteByte(int value);

/**
 * @brief Writes a character to a message buffer.
 *
 * @note Use after MessageBegin().
 *
 * @param value        Character to write.
 *
 * @noreturn
 */
native void WriteChar(char value);

/**
 * @brief Writes a short integer to a message buffer.
 *
 * @note Use after MessageBegin().
 *
 * @param value        Integer to write (value will be written as 16bit).
 *
 * @noreturn
 */
native void WriteShort(int value);

/**
 * @brief Writes a long integer to a message buffer.
 *
 * @note Use after MessageBegin().
 *
 * @param value        Integer to write (value will be written as 32bit).
 *
 * @noreturn
 */
native void WriteLong(int value);

/**
 * @brief Writes an entity to a message buffer.
 *
 * @note Use after MessageBegin().
 *
 * @param value        Entity index to write.
 *
 * @noreturn
 */
native void WriteEntity(int value);

/**
 * @brief Writes an angle to a message buffer.
 *
 * @note Use after MessageBegin().
 *
 * @param value        Angle to write.
 *
 * @noreturn
 */
native void WriteAngle(int value);

/**
 * @brief Writes a float angle to a message buffer.
 *
 * @note Use after MessageBegin().
 *
 * @param value        Float angle to write.
 *
 * @noreturn
 */
native void WriteAngleF(float value);

/**
 * @brief Writes a coordinate to a message buffer.
 *
 * @note Use after MessageBegin().
 *
 * @param value        Coordinate to write.
 *
 * @noreturn
 */
native void WriteCoord(int value);

/**
 * @brief Writes a float coordinate to a message buffer.
 *
 * @note Use after MessageBegin().
 *
 * @param value        Float coordinate to write.
 *
 * @noreturn
 */
native void WriteCoordF(float value);

/**
 * @brief Writes a string to a message buffer.
 *
 * @note Use after MessageBegin().
 *
 * @param value        Text string to write.
 *
 * @noreturn
 */
native void WriteString(const char[] value);
