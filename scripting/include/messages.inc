/*
 *  Copyright (C) 2018-2019 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SPMod is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with SPMod.  If not, see <https://www.gnu.org/licenses/>.
 */

#if defined _messages_included
    #endinput
#endif
#define _messages_included

/**
 * Destination types for MessageBegin()
 */

enum MessageDest
{
    MSG_BROADCAST = 0,          // Unreliable to all
    MSG_ONE = 1,                // Reliable to one (msg_entity)
    MSG_ALL = 2,                // Reliable to all
    MSG_INIT = 3,               // Write to the init string
    MSG_PVS = 4,                // Ents in PVS of org
    MSG_PAS = 5,                // Ents in PAS of org
    MSG_PVS_R = 6,              // Reliable to PVS
    MSG_PAS_R = 7,              // Reliable to PAS
    MSG_ONE_UNRELIABLE = 8,     // Send to one client, but don't put in reliable stream, put in unreliable datagram (could be dropped)
    MSG_SPEC = 9                // Sends to all spectator proxies
};

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

enum MessageHook
{
    INVALID_MSG_HOOK = -1
};

enum MsgArgType
{
    MsgArg_Byte = 1,
    MsgArg_Char,
    MsgArg_Short,
    MsgArg_Long,
    MsgArg_Angle,
    MsgArg_Coord,
    MsgArg_String,
    MsgArg_Entity,
};

enum BlockType
{
    MessageBlock_Not,
    MessageBlock_Once,
    MessageBlock_Set
}

typedef MessageHandler = function PluginReturn (MessageDest dest, int type, int receiver);

/**
 * @brief Hooks a user message.
 *
 * @param msg_type          Message index.
 * @param hook              Function to use as a hook.
 * @param post              Post or not.
 *
 * @return                  Hook index.
 */
native MessageHook HookMessage(int msg_type, MessageHandler hook, bool post = false);

/**
 * @brief Unhooks a user message.
 *
 * @param hook              Hook index.
 *
 * @noreturn
 */
native void UnhookMessage(MessageHook hook);

/**
 * @brief Gets a user message block type.
 *
 * @param msg_type          Message index.
 *
 * @return                  Message block type.
 */
native BlockType GetMsgBlock(int msg_type);

/**
 * @brief Sets a user message block type.
 *
 * @param msg_type          Message index.
 * @param type              Block type.
 *
 * @noreturn
 */
native void SetMsgBlock(int msg_type, BlockType type);

/**
 * @brief Gets number of arguments that were passed to a message.
 *
 * @return                  Number of arguments.
 */
native int GetMsgArgs();

/**
 * @brief  Gets the argument type.
 *
 * @param argn              Argument number.
 *
 * @return                  Argument type.
 */
native MsgArgType GetMsgArgType(int argn);

/**
 * @brief  Gets the integer value of a argument.
 *
 * @param argn              Argument number.
 *
 * @return                  Argument type.
 */
native int GetMsgArgInt(int argn);

/**
 * @brief  Gets the float value of a argument.
 *
 * @param argn              Argument number.
 *
 * @return                  Argument type.
 */
native float GetMsgArgFloat(int argn);

/**
 * @brief  Gets the string value of a argument.
 *
 * @param argn              Argument number.
 * @param buffer            Buffer for string.
 * @param size              Buffer size.
 *
 * @return                  String length.
 */
native int GetMsgArgString(int argn, char[] buffer, int size);

/**
 * @brief  Sets the integer value of a argument.
 *
 * @param argn              Argument number.
 * @param value             Argument value.
 *
 * @noreturn
 */
native void SetMsgArgInt(int argn, int value);

/**
 * @brief  Sets the float value of a argument.
 *
 * @param argn              Argument number.
 * @param value             Argument value.
 *
 * @noreturn
 */
native void SetMsgArgFloat(int argn, float value);

/**
 * @brief  Sets the string value of a argument.
 *
 * @param argn              Argument number.
 * @param string            Argument string.
 *
 * @noreturn
 */
native void SetMsgArgString(int argn, const char[] string);

/**
 * @brief Starts a message.
 *
 * @param dest          Destination type.
 * @param msgid         Message index.
 * @param origin        Message origin.
 * @param player        Player index to broadcast to.
 * @param ignore_hooks  Ignore this message in message hooks.
 *
 * @noreturn
 * @error               Invalid message index, invalid number of parameters passed.
 */
native void MessageBegin(MessageDest dest, int msgid, float origin[3] = {0.0,0.0,0.0}, int player = 0, bool ignore_hooks = true);

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
native void WriteAngle(float value);

/**
 * @brief Writes a coordinate to a message buffer.
 *
 * @note Use after MessageBegin().
 *
 * @param value        Coordinate to write.
 *
 * @noreturn
 */
native void WriteCoord(float value);

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
