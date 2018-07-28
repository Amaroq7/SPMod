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

#if defined _string_included
    #endinput
#endif
#define _string_included

/**
 * @brief Converts integer to string.
 *
 * @param num       Number to be converted.
 * @param buffer    Buffer where the converted number will be written to.
 * @param size      Size of the buffer.
 *
 * @return          Number of characters written to buffer.
 */
native int NumToString(int num, char[] buffer, int size);

/**
 * @brief Converts real to string.
 *
 * @param real      Real to be converted.
 * @param buffer    Buffer where the converted number will be written to.
 * @param size      Size of the buffer.
 *
 * @return          Number of characters written to buffer.
 */
native int RealToString(float real, char[] buffer, int size);

/**
 * @brief Copies string to buffer.
 *
 * @param buffer    Buffer where copy string to.
 * @param size      Size of the buffer.
 * @param source    String to be copied.
 *
 * @return          Number of characters written to buffer.
 */
native int CopyString(char[] buffer, int size, const char[] source);