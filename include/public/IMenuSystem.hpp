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

#include "IPlayerSystem.hpp"

namespace SPMod
{
    enum class ItemStatus
    {
        Enabled,
        Disabled,
        Hide
    };

    enum class NavigationType
    {
        None,
        Back,
        Next,
        Exit
    };

    class IMenu;

    class IMenuItem SPMOD_FINAL
    {
    public:
        using MenuItemCallback = ItemStatus (*)(IMenu *const menu, IMenuItem *const item, IPlayer *const player);

        virtual const char *getName() const = 0;
        virtual void setName(const char *name) = 0;

        virtual void *getData() const = 0;
        virtual void setData(void *data) = 0;

        virtual NavigationType getNavType() const = 0;

        virtual void setCallback(MenuItemCallback func) = 0;
    
    protected:
        virtual ~IMenuItem() = default;
    };

    class IMenu SPMOD_FINAL
    {
    public:
        using MenuItemCallback = ItemStatus (*)(IMenu *const menu, IMenuItem *const item, IPlayer *const player);
        using MenuItemHandler = void (*)(IMenu *const menu, IMenuItem *const item, IPlayer *const player);
        using MenuTextHandler = void (*)(IMenu *const menu, int key, IPlayer *const player);

        enum class Style
        {
            Item,
            Text
        };

        virtual void display(IPlayer *const player,
                            int page,
                            int time) = 0;
        virtual bool getGlobal() const = 0;
        virtual Style getStyle() const = 0;

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
        
        virtual bool setStaticItem(std::size_t position,
                                   const char *name,
                                   MenuItemCallback callback,
                                   void *data) = 0;
        
        virtual bool removeItem(std::size_t position) = 0;
        virtual void removeAllItems() = 0;

        virtual IMenuItem *getItem(std::size_t position) const = 0;

        virtual std::size_t getItems() const = 0;

    protected:
        virtual ~IMenu() = default;
    };

    class IMenuMngr SPMOD_FINAL
    {
    public:
        using MenuItemHandler = void (*)(IMenu *const menu, IMenuItem *const item, IPlayer *const player);
        using MenuTextHandler = void (*)(IMenu *const menu, int key, IPlayer *const player);

        virtual IMenu *registerMenu(MenuItemHandler handler, bool global) = 0;
        virtual IMenu *registerMenu(MenuTextHandler handler, bool global) = 0;
        virtual void destroyMenu(IMenu *menu) = 0;
    protected:
        virtual ~IMenuMngr() = default;
    };
}

