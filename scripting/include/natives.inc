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

#if defined _natives_included
    #endinput
#endif
#define _natives_included

/*
 * @brief Callback function for native.
 * 
 * @param plugin_id Id of caller plugin.
 * 
 * @return          Cell value.
 */
typedef PluginNative = function int (int plugin_id);

/**
 * @brief Registers plugin's native.
 *
 * @param name      Name of the native.
 * @param func      Id of callback function.
 * 
 * @return          True if native has been successfully added, false otherwise.
 */
native bool NativeRegister(const char[] name, PluginNative func);

/**
 * @brief Gets a cell from a native parameter.
 * 
 * @note To get the number of parameters passed to the native call nativeGetCell(0).
 *
 * @param param     Parameter number.
 *
 * @return          Cell value at the parameter number.
 * @error           Invalid parameter number or calling from a non-native function.
 */
native any NativeGetCell(int param);

/**
 * @brief Gets a cell by reference from a native parameter.
 *
 * @param param     Parameter number.
 *
 * @return          Cell value at the parameter number.
 * @error           Invalid parameter number or calling from a non-native function.
 */
native any NativeGetCellRef(int param);

/**
 * @brief Gets a string from a native parameter.
 *
 * @param param     Parameter number.
 * @param buffer    Buffer where the string will be stored to.
 * @param size      Size of the buffer.
 *
 * @return          Number of bytes written to the buffer.
 * @error           Invalid parameter number or calling from a non-native function.
 */
native int NativeGetString(int param, char[] buffer, int size);

/**
 * @brief Gets an array from a native parameter.
 *
 * @param param     Parameter number.
 * @param buffer    Buffer where the array will be stored to.
 * @param size      Size of the buffer.
 *
 * @return          True if succeed, false otherwise.
 * @error           Invalid parameter number or calling from a non-native function.
 */
native bool NativeGetArray(int param, any[] buffer, int size);

/**
 * @brief Sets a cell by reference in a native parameter.
 *
 * @param param     Parameter number.
 * @param value     Value which parameter will be set to.
 *
 * @return          True if succeed, false otherwise.
 * @error           Invalid parameter number or calling from a non-native function.
 */
native bool NativeSetCellRef(int param, any value);

/**
 * @brief Sets a string in a native parameter.
 *
 * @param param     Parameter number.
 * @param string    String to be set.
 * @param size      Number of bytes to be written.
 *
 * @return          Number of bytes written.
 * @error           Invalid parameter number or calling from a non-native function.
 */
native int NativeSetString(int param, const char[] string, int size);

/**
 * @brief Sets an array in a native parameter.
 *
 * @param param     Parameter number.
 * @param buffer    Array to be set.
 * @param size      Size of the array.
 *
 * @return          True if succeed, false otherwise.
 * @error           Invalid parameter number or calling from a non-native function.
 */
native bool NativeSetArray(int param, const any[] buffer, int size);
