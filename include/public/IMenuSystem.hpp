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
    class IPlayer;

    enum class NavigationType : int8_t
    {
        None = 0,
        Back = -1,
        Next = -2,
        Exit = -3
    };

    class IMenu
    {
    public:
        enum class Style : int8_t
        {
            Item = 0,
            Text
        };

        class IItem
        {
        public:
            enum class Status : int8_t
            {
                Enabled = 0,
                Disabled,
                Hide
            };

            using Callback = Status (*)(IMenu *const menu, IItem *const item, IPlayer *const player, void *data);

            virtual const char *getName() const = 0;
            virtual void setName(const char *name) = 0;

            virtual void *getData() const = 0;
            virtual void setData(void *data) = 0;

            virtual NavigationType getNavType() const = 0;

            virtual void setCallback(Callback func, void *data) = 0;

        protected:
            virtual ~IItem() = default;
        };

        using ItemHandler = void (*)(IMenu *const menu, IItem *const item, IPlayer *const player, void *data);
        using TextHandler = void (*)(IMenu *const menu, int key, IPlayer *const player, void *data);

        virtual void display(IPlayer *const player, int page, int time) = 0;
        virtual bool getGlobal() const = 0;
        virtual Style getStyle() const = 0;

        virtual void setText(const char *text) = 0;
        virtual void setKeys(int keys) = 0;

        virtual void setTitle(const char *text) = 0;

        virtual void setItemsPerPage(std::size_t value) = 0;
        virtual std::size_t getItemsPerPage() const = 0;

        virtual int getTime() const = 0;
        virtual int getKeys() const = 0;

        virtual IItem *appendItem(const char *name, IItem::Callback callback, void *cbData, void *data) = 0;

        virtual IItem *
            insertItem(std::size_t position, const char *name, IItem::Callback callback, void *cbData, void *data) = 0;

        virtual IItem *setStaticItem(std::size_t position,
                                     const char *name,
                                     IItem::Callback callback,
                                     void *cbData,
                                     void *data) = 0;

        virtual bool removeItem(std::size_t position) = 0;
        virtual void removeAllItems() = 0;

        virtual IItem *getItem(std::size_t position) const = 0;
        virtual int getItemIndex(const IItem *item) const = 0;
        virtual void setNumberFormat(const char *format) = 0;

        virtual std::size_t getItems() const = 0;

    protected:
        virtual ~IMenu() = default;
    };

    class IMenuMngr : public ISPModInterface
    {
    public:
        static constexpr std::uint16_t MAJOR_VERSION = 0;
        static constexpr std::uint16_t MINOR_VERSION = 0;

        static constexpr std::uint32_t VERSION = (MAJOR_VERSION << 16 | MINOR_VERSION);
        /**
         * @brief Gets interface's name.
         *
         * @return        Interface's name.
         */
        const char *getName() const override
        {
            return "IMenuMngr";
        }

        /**
         * @brief Gets interface's version.
         *
         * @note The first 16 most significant bits represent major version, the rest represent minor version.
         *
         * @return        Interface's version.
         */
        std::uint32_t getVersion() const override
        {
            return VERSION;
        }

        virtual IMenu *registerMenu(IMenu::ItemHandler handler, void *data, bool global) = 0;
        virtual IMenu *registerMenu(IMenu::TextHandler handler, void *data, bool global) = 0;
        virtual void destroyMenu(IMenu *menu) = 0;

    protected:
        virtual ~IMenuMngr() = default;
    };
} // namespace SPMod
