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

#if defined _spmod_included
    #endinput
#endif
#define _spmod_included

// Do not allow plugins to use old syntax
#pragma newdecls required

/*
 * @brief Predefined return results.
 */
enum PluginReturn
{
    PluginContinue = 0,
    PluginStop,
    PluginHandled
}

#include <clients>
#include <cvars>
#include <natives>
#include <forwards>
#include <string>
#include <messages>
#include <commands>
#include <timers>
#include <menus>
#include <float>

/*
 * @brief Provides info about plugin.
 *        Must be present, otherwise plugin will not be loaded.
 */
struct PluginInfo
{
	public const char[] name;
	public const char[] version;
	public const char[] author;
	public const char[] url;
};

/*
 * @brief Maximum number of players the server supports.
 */
public const int maxClients;

/*
 * @brief Called before OnPluginInit, plugin is supposed to add any of its natives here.
 *        Only registering natives should take place here.
 */
forward void OnPluginNatives();

/*
 * @brief Called when plugin has been loaded. Paired with OnPluginEnd().
 */
forward void OnPluginInit();

/*
 * @brief Called when all plugins has been loaded.
 */
forward void OnPluginsLoaded();

/*
 * @brief Called when plugin is about to be unloaded.
 */
forward void OnPluginEnd();

/*
 * @brief Called when map is going to change.
 */
forward PluginReturn OnMapChange(const char[] map);

/*
 * @brief Prints text to the server's console.
 * 
 * @param text      Text to be printed.
 * @param ...       Format parameters.
 * 
 * @noreturn
 */
native void PrintToServer(const char[] text, any ...);

/**
 * @brief Precaches a model file.
 *
 * @note Can be called inside the OnPluginInit() forward.
 *
 * @param model     Path to the model file.
 *
 * @return          Cache id of the model.
 * @error           If called outside of the OnPluginInit() forward.
 */
native int PrecacheModel(const char[] model);

/**
 * @brief Precaches a sound file.
 *
 * @note Can be called inside the OnPluginInit() forward.
 * @note Precached files have to be wav ones. To precache other types use
 *       precacheGeneric().
 *
 * @param sound     Path to the sound file (relative to "sound" folder).
 *
 * @return          Cache id of the sound.
 * @error           If called outside of the OnPluginInit() forward.
 */
native int PrecacheSound(const char[] sound);

/**
 * @brief Precaches a generic file.
 *
 * @note Can be called inside the OnPluginInit() forward.
 *
 * @param generic   Path to the generic file.
 *
 * @return          Cache id of the file.
 * @error           If called outside of the OnPluginInit() forward.
 */
native int PrecacheGeneric(const char[] generic);

/**
 * @brief Changes a map.
 *
 * @param map       Map to change.
 *
 * @noreturn
*/
native void ChangeLevel(const char[] map);

/**
 * @brief Returns the game time based on the game tick.
 *
 * @note This time is counted up from map start. If the engine is not processing
 *       this function will return the same value between calls, which makes it
 *       unusable for profiling purposes.
 *
 * @return      Game time, in seconds
 */
native float GetGameTime();
