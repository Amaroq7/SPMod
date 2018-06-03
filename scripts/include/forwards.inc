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

#if defined _forwards_included
    #endinput
#endif
#define _forwards_included

/*
 * Execution types
 */
enum ExecType
{
    /* Ignore returned result */
    Ignore = 0,

    /* Stop executing when plugin return ReturnValue::PluginStop */
    Stop = (1 << 0),

    /* Return the highest value returned by plugins */
    Highest = (1 << 1)
};

/*
 * Param types
 */
enum ParamType
{
    None = 0,
    Cell,
    CellRef,
    Float,
    FloatRef,
    Array,
    String
};

/*
 * String flags for StringEx param
 */
enum StringFlags
{
    /* No special behavior */
    None = 0,

    /* String should be UTF-8 handled */
    Utf8 = (1 << 0),

    /* String should be copied into the plugin */
    Copy = (1 << 1),

    /* String should be handled as binary data */
    Binary = (1 << 2)
};

enum Forward
{
    INVALID_FORWARD = -1
};

methodmap Forward
{
    /*
     * @brief Creates a forward.
     * 
     * @note If plid is supplied, then exectype param is ignored.
     * 
     * @param name      Name of the forward.
     * @param exectype  Bitsum of exec type.
     * @param plid      Plugin id where the forward will be executed in, -1 if it is multi forward.
     * @param ...       Parameters types (max 32).
     * 
     * @return          Forward id, -1 if creation failed.
     */
    public native Forward(const char[] name, ExecType exectype, int plid, ParamType ...);

    /*
     * @brief Pushes cell.
     * 
     * @param cell      Cell value to push.
     * 
     * @return          True if succeed, false otherwise.
     */
    public native bool pushCell(any cell);

    /*
     * @brief Pushes cell by reference.
     * 
     * @param cell      Cell value to push.
     * 
     * @return          True if succeed, false otherwise.
     */
    public native bool pushCellRef(any &cell);

    /*
     * @brief Pushes real number.
     * 
     * @param real      Real value to push.
     * 
     * @return          True if succeed, false otherwise.
     */
    public native bool pushFloat(float real);

    /*
     * @brief Pushes real number by reference.
     * 
     * @param real      Real value to push.
     * 
     * @return          True if succeed, false otherwise.
     */
    public native bool pushFloatRef(float &real);

    /*
     * @brief Pushes string.
     * 
     * @param string    String to push.
     * 
     * @return          True if succeed, false otherwise.
     */
    public native bool pushString(const char[] string);

    /*
     * @brief Pushes array.
     * 
     * @param array     Array to push.
     * @param size      Size of the array.
     * 
     * @return          True if succeed, false otherwise.
     */
    public native bool pushArray(const any[] array, int size);

    /*
     * @brief Pushes string.
     * 
     * @param string    String to push.
     * @param length    Length of the string.
     * @param sflags    String flags.
     * @param copyback  True to copyback string, false to not.
     * 
     * @return          True if succeed, false otherwise.
     */
    public native bool pushStringEx(char[] string, int length, StringFlags sflags, bool copyback);

    /*
     * @brief Pushes array.
     * 
     * @param array     Array to push.
     * @param size      Size of the array.
     * @param copyback  True to copyback array, false to not.
     * 
     * @return          True if succeed, false otherwise.
     */
    public native bool pushArrayEx(any[] array, int size, bool copyback);

    /*
     * @brief Executes forward.
     * 
     * @param result    Variable which store the returned value to.
     * 
     * @return          True if succeed, false otherwise.
     */
    public native bool pushExec(any &result = 0);

    /*
     * @brief Resets already pushed params.
     * 
     * @return          True if succeed, false otherwise.
     */
    public native bool pushCancel();

    /*
     * @brief Deletes forward.
     * 
     * @note Cannot be deleted while being executed.
     * 
     * @return          True if succeed, false otherwise.
     */
    public native bool remove();
};

/*
 * @brief Finds the forward by name.
 * 
 * @param name      Forward name to look for.
 * 
 * @return          Forward or INVALID_FORWARD if not found.
 */
native Forward findForward(const char[] name);