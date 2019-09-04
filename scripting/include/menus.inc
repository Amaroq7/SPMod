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

#if defined _menus_included
    #endinput
#endif
#define _menus_included

enum Menu
{
    INVALID_MENU = -1
};

enum MenuStyle
{
    MenuItemStyle = 0,
    MenuTextStyle
}

enum MenuProp
{
    /**
     * @param format            Should contain "#num", example "\\r[#num]"
     */
    MProp_NumberFormat
};

enum MenuItem
{
    MenuExit = -3,
    MenuNext = -2,
    MenuBack = -1
};

enum ItemStatus
{
    ItemEnabled,
    ItemDisabled,
    ItemHide
};

typeset MenuHandler
{
    /**
    * Called after choosing menu key.
    *
    * @note Used in TextStyle menu
    *
    * @param menu           Menu index.
    * @param key            Pressed menu key.
    * @param player         Client index.
    */
    function void (Menu menu, int key, Player player);

    /**
    * Called after selection menu item.
    *
    * @note Used in ItemStyle menu
    *
    * @param menu           Menu index.
    * @param item           Selected menu item.
    * @param player         Client index.
    */
    function void (Menu menu, MenuItem item, Player player);
};

typedef CallbackHandler = function ItemStatus (Menu menu, MenuItem item, Player player);

methodmap Menu
{
    /*
     * @brief Creates a menu.
     * 
     * @param handler       Name of the forward.
     * @param style         Menu style.
     * @param global        If true then menu don't be destroyed after choose item.
                            TextStyle menu always global.
     * 
     * @return              Menu id, -1 if creation failed.
     */
    public native Menu(MenuHandler handler, MenuStyle style = MenuItemStyle, bool global = false);
    /*
     * @brief Show a menu to player.
     * 
     * @param player        Client index.
     * @param page          Page to start from (starting from 0).
     * @param time          If >=0 menu will timeout after this many seconds
     * 
     * @noreturn
     */
    public native void Display(int player, int page = 0, int time = -1);
    /*
     * @brief Destroys a menu.
     *
     * @note Use only for global menu
     * 
     * @noreturn
     */
    public native void Destroy();

    // Natives only for item style

    /*
     * @brief Sets a menu title.
     *
     * @param title         Title text.
     * 
     * @noreturn
     */
    public native void SetTitle(const char[] title);
    /*
     * @brief Adds a menu item to end of items list.
     *
     * @param name          Item text.
     * @param data          Item data.
     * @param callback      Item callback
     * 
     * @noreturn
     */
    public native void AddItem(const char[] name, any data = 0, CallbackHandler callback = INVALID_FUNCTION);
    /*
     * @brief Adds a menu static item. This item will be draw on each page of menu in same position.
     *
     * @param position      Item position, from 0 to Navigation item(Next, Back, Exit) usually starts from 7
     * @param name          Item text.
     * @param data          Item data.
     * @param callback      Item callback
     * 
     * @noreturn
     */
    public native void AddStaticItem(int position, const char[] name, any data = 0, CallbackHandler callback = INVALID_FUNCTION);
    /*
     * @brief Adds a menu item.
     *
     * @param position      Item position, from 0 to Menu.Items.
     * @param name          Item text.
     * @param data          Item data.
     * @param callback      Item callback
     * 
     * @noreturn
     */
    public native void InsertItem(int position, const char[] name, any data = 0, CallbackHandler callback = INVALID_FUNCTION);
    /*
     * @brief Removes a menu item.
     *
     * @param position      Item position, from 0 to Menu.Items.
     * 
     * @noreturn
     */
    public native void RemoveItem(int position);
    /*
     * @brief Removes all menu item.
     * 
     * @noreturn
     */
    public native void RemoveAllItems();
    /*
     * @brief Sets a menu property.
     *
     * @param prop          Menu property.
     * @param ...           Property args.
     * 
     * @noreturn
     */
    public native void SetProp(MenuProp prop, any ...);
    /*
     * @brief Gets a menu items count.
     * 
     */
    property int Items {
        public native get();
    }
    /*
     * @brief Gets or sets a menu items per page.
     * 
     */
    property int ItemsPerPage {
        public native get();
        public native set(int value);
    }
    
    // only for text tyle

    /*
     * @brief Sets a menu text.
     *
     * @param text          Menu text.
     * 
     * @noreturn
     */
    public native void SetText(const char[] text);
    /*
     * @brief Sets a menu keys.
     *
     * @param keys          Menu keys, bitsum.
     * 
     * @noreturn
     */
    public native void SetKeys(int keys);
}

methodmap MenuItem
{
    /*
     * @brief Gets a item name.
     *
     * @param name          Name buffer.
     * @param size          Buffer size.
     * 
     * @return              Return chars written to buffer.
     */
    public native int GetName(char[] name, int size);
    /*
     * @brief Sets a item name.
     *
     * @param name          Name buffer.
     *
     * @noreturn
     */
    public native void SetName(const char[] name);
    /*
     * @brief Gets a item data.
     * 
     * @return              Item data.
     */
    public native any GetData();
    /*
     * @brief Sets a item data.
     *
     * @param data          Item data.
     *
     * @noreturn
     */
    public native void SetData(any data);
}

/*
* @brief Closes any menu for player.
*
* @param player         Client index.
* 
* @noreturn
*/
native void CloseMenu(int player);
