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
    enum class NavigationType : int8_t
    {
        None = 0,
        Back = -1,
        Next = -2,
        Exit = -3
    };

    class IMenu SPMOD_FINAL
    {
    public:
        enum class Style : int8_t
        {
            Item = 0,
            Text
        };

        class IItem SPMOD_FINAL
        {
        public:
            enum class Status : int8_t
            {
                Enabled = 0,
                Disabled,
                Hide
            };

            using Callback = Status (*)(IMenu *const menu, IItem *const item, IPlayer *const player);

            virtual const char *getName() const = 0;
            virtual void setName(const char *name) = 0;

            virtual void *getData() const = 0;
            virtual void setData(void *data) = 0;

            virtual NavigationType getNavType() const = 0;

            virtual void setCallback(Callback func) = 0;
        
        protected:
            virtual ~IItem() = default;
        };

        using ItemHandler = void (*)(IMenu *const menu, IItem *const item, IPlayer *const player);
        using TextHandler = void (*)(IMenu *const menu, int key, IPlayer *const player);

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

        virtual IItem *appendItem(const char *name,
                                  IItem::Callback callback,
                                  void *data) = 0;
        
        virtual IItem *insertItem(std::size_t position,
                                  const char *name,
                                  IItem::Callback callback,
                                  void *data) = 0;
        
        virtual IItem *setStaticItem(std::size_t position,
                                     const char *name,
                                     IItem::Callback callback,
                                     void *data) = 0;
        
        virtual bool removeItem(std::size_t position) = 0;
        virtual void removeAllItems() = 0;

        virtual IItem *getItem(std::size_t position) const = 0;

        virtual std::size_t getItems() const = 0;

    protected:
        virtual ~IMenu() = default;
    };

    class IMenuMngr SPMOD_FINAL
    {
    public:
        virtual IMenu *registerMenu(IMenu::ItemHandler handler, bool global) = 0;
        virtual IMenu *registerMenu(IMenu::TextHandler handler, bool global) = 0;
        virtual void destroyMenu(IMenu *menu) = 0;

    protected:
        virtual ~IMenuMngr() = default;
    };
}

