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

#pragma once

namespace SPMod
{
    enum
    {
        MENU_EXIT = -3,
        MENU_NEXT = -2,
        MENU_BACK = -1
    };

    enum class ItemStatus
    {
        Enabled,
        Disabled,
        Hide
    };

    enum class MenuStyle
    {
        Item,
        Text
    };
    
    class IMenu SPMOD_FINAL
    {
    public:
        using MenuItemCallback = ItemStatus (*)(IMenu *const menu, std::size_t item, int player);
        using MenuHandler = void (*)(IMenu *const menu, std::size_t item, int player);

        virtual void display(int player,
                            int page,
                            int time) = 0;
        virtual bool getGlobal() const = 0;
        virtual MenuStyle getStyle() const = 0;

        virtual void setText(const char *text) = 0;
        virtual void setKeys(int keys) = 0;

        virtual void setTitle(const char *text) = 0;

        virtual void setItemsPerPage(std::size_t value) = 0;
        virtual std::size_t getItemsPerPage() const = 0;

        virtual int getTime() const = 0;
        virtual int getKeys() const = 0;

        virtual void appendItem(const char *name,
                                MenuItemCallback callback,
                                void *data) = 0;
        
        virtual bool insertItem(std::size_t position,
                                const char *name,
                                MenuItemCallback callback,
                                void *data) = 0;
        
        virtual bool removeItem(std::size_t position) = 0;
        virtual void removeAllItems() = 0;

        virtual const char *getItemName(std::size_t item) const = 0;
        virtual bool setItemName(std::size_t item,
                                 const char *name) = 0;
        
        virtual void *getItemData(std::size_t position) const = 0;
        virtual void setItemData(std::size_t position,
                                 void *data) = 0;

        virtual std::size_t getItems() const = 0;

    protected:
        virtual ~IMenu() {}
    };

    class IMenuMngr SPMOD_FINAL
    {
    public:
        using MenuHandler = void (*)(IMenu *const menu, std::size_t item, int player);
        virtual IMenu *registerMenu(MenuHandler handler, MenuStyle style, bool global) = 0;
        virtual IMenu *findMenu(std::size_t mid) const = 0;
    protected:
        virtual ~IMenuMngr() {}
    };
}

