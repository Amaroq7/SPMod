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

enum Cvar
{
    INVALID_CVAR = -1
};

// Called when a console variable's value is changed.
// 
// @param convar		Handle to the convar that was changed.
// @param oldValue		String containing the value of the convar before it was changed.
// @param newValue		String containing the new value of the convar.
typedef ConVarChanged = function void (Cvar convar, const char[] oldValue, const char[] newValue);

methodmap Cvar
{
	property int IntValue {
		public native get();
		public native set(int value);
	}
	property float FloatValue {
		public native get();
		public native set(float value);
	}
	property int Flags {
		public native get();
		public native set(int flags);
    }
    /* this constructor would register cvar */
    public native Cvar(const char[] name, const char[] value, CvarFlags flags=None);
    public native void GetName(char[] name, int size);
    public native float GetFloat();
    public native void GetString(char[] value, int size);
    public native int GetInt();
    public native CvarFlags GetFlags();
    public native void SetFloat(float value);
    public native void SetString(const char[] value);
    public native void SetInt(int value);
    public native void SetFlags(CvarFlags flags);
    public native void AddHookOnChange(ConVarChanged callback);
};
native Cvar FindCvar(const char[] name);