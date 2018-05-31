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

#if defined _cvars_included
    #endinput
#endif
#define _cvars_included

/*
 * Cvar flags
 */
enum CvarFlags
{
    // None
    None = 0,

    // set to cause it to be saved to vars.rc
    Archive = (1<<0),

    // changes the client's info string
    UserInfo = (1<<1),

    // notifies players when changed
    Server = (1<<2),

    // defined by external DLL
    ExtDll = (1<<3),

    // defined by the client dll
    ClientDll = (1<<4),

    // It's a server cvar, but we don't send the data since it's a password, etc.
    // Sends 1 if it's not bland/zero, 0 otherwise as value
    Protected = (1<<5),     

    // This cvar cannot be changed by clients connected to a multiplayer server.
    SPOnly = (1<<6),

    // This cvar's string cannot contain unprintable characters (e.g., used for player name etc).
    PrintableOnly = (1<<7),
	
    // If this is a Server (FCVAR_SERVER), don't log changes to the log file / console if we are creating a log
    Unlogged = (1<<8),

    // strip trailing/leading white space from this cvar
    NoExtraWhitepace = (1<<9)
}

/*
 * @brief Called on every cvar change.
 * 
 * @param cvar      Cvar name.
 * @param oldval    Old value.
 * @param newval    New value.
 * @param newnum    New value as float if newvalue is number, otherwise it is 0.0.
 * 
 * @noreturn
 */
forward void OnCvarChange(const char[] cvar, const char[] oldval, const char[] newval, float newnum);

/**
 * @brief Returns cvar value as real number.
 *
 * @param name      Cvar name.
 *
 * @return          Cvar real value.
 */
native float cvarGetFloat(const char[] name);

/**
 * @brief Gets cvar value as string.
 *
 * @param name      Cvar name.
 * @param value     Buffer to copy the cvar value to.
 * @param size      Size of the buffer.
 *
 * @noreturn
 */
native void cvarGetString(const char[] name, char[] value, int size);

/**
 * @brief Returns cvar value as integer.
 *
 * @param name      Cvar name.
 *
 * @return          Cvar integer value.
 */
native int cvarGetInt(const char[] name);

/**
 * @brief Returns flags of a cvar.
 *
 * @param name      Cvar name.
 *
 * @return          Bitsum of cvar flags.
 */
native CvarFlags cvarGetFlags(const char[] name);

/**
 * @brief Sets a real value for a cvar.
 *
 * @param name      Cvar name.
 * @param value     Value to set.
 *
 * @noreturn
 */
native void cvarSetFloat(const char[] name, float value);

/**
 * @brief Sets a string value for a cvar.
 *
 * @param name      Cvar name.
 * @param value     Value to set.
 *
 * @noreturn
 */
native void cvarSetString(const char[] name, const char[] value);

/**
 * @brief Sets a integer value for a cvar.
 *
 * @param name      Cvar name.
 * @param value     Value to set.
 *
 * @noreturn
 */
native void cvarSetInt(const char[] name, int value);

/**
 * @brief Sets specified flags for a cvar.
 *
 * @note For available flags see the top of this file.
 *
 * @param name      Cvar name.
 * @param flags     Bitsum of flags to set.
 *
 * @noreturn
 */
native void cvarSetFlags(const char[] name, CvarFlags flags);

/**
 * @brief Registers a new cvar.
 *
 * @note For available flags see the top of this file.
 *
 * @param name      Cvar name.
 * @param value     Value to set.
 * @param flags     Bitsum of flags to set.
 *
 * @noreturn
 */
native void cvarRegister(const char[] name, const char[] value, CvarFlags flags = None);
