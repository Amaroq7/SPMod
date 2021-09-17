/*
 *  Copyright (C) 2018-2020 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  SPMod is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with SPMod.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <Common.hpp>
#include <IMenuSystem.hpp>
#include "MetaInit.hpp"

using namespace SPMod;

constexpr std::uint32_t MAX_STATIC_ITEMS = 10U;

class Player;

class Menu final : public IMenu
{
public:
    class Item final : public IMenu::IItem
    {
    public:
        Item(Menu *menu, std::string_view name, Callback callback, std::any data, NavigationType type);

        ~Item() = default;

        // IItem
        std::string_view getName() const override;
        void setName(std::string_view name) override;

        std::any getData() const override;
        void setData(std::any data) override;

        NavigationType getNavType() const override;

        // Item
        Item::Status execCallback(IPlayer *player);

    private:
        Menu *m_menu;
        std::string m_name;
        Callback m_callback;
        std::any m_data;
        NavigationType m_type;
    };

    Menu(const std::variant<ItemHandler, TextHandler> &handler, Menu::Style style, bool global);
    ~Menu() = default;

    // IMenu
    void display(IPlayer *player, std::uint32_t page, std::uint32_t time) override;

    bool getGlobal() const override;
    Menu::Style getStyle() const override;

    void setText(std::string_view text) override;
    void setKeys(std::uint32_t keys) override;
    void setTitle(std::string_view text) override;

    void setItemsPerPage(std::size_t value) override;
    std::size_t getItemsPerPage() const override;

    std::uint32_t getTime() const override;
    std::uint32_t getKeys() const override;

    Item *appendItem(std::string_view name, Item::Callback callback, std::any data) override;

    Item *insertItem(std::size_t position,
                     std::string_view name,
                     Item::Callback callback,
                     std::any data) override;

    Item *setStaticItem(std::size_t position,
                        std::string_view name,
                        Item::Callback callback,
                        std::any data) override;

    bool removeItem(std::size_t position) override;
    void removeAllItems() override;

    Item *getItem(std::size_t position) const override;
    std::uint32_t getItemIndex(const IItem *item) const override;
    void setNumberFormat(std::string_view format) override;

    std::size_t getItems() const override;

    // Menu
    Item *keyToItem(std::uint32_t key) const;

    void execTextHandler(Player *player, std::uint32_t key);
    void execItemHandler(Player *player, Item *item);
    void execExitHandler(Player *player);

private:
    Item *_addItem(std::uint32_t position,
                   std::string_view name,
                   Item::Callback callback,
                   std::any data);

private:
    Menu::Style m_style;
    bool m_global;
    std::string m_text;
    std::string m_title;
    std::string m_numberFormat;
    std::uint32_t m_time;
    std::size_t m_itemsPerPage;
    std::uint32_t m_keys;

    std::array<Item *, 10> m_slots;
    std::array<std::unique_ptr<Item>, MAX_STATIC_ITEMS> m_staticItems;

    std::unique_ptr<Item> m_nextItem;
    std::unique_ptr<Item> m_backItem;
    std::unique_ptr<Item> m_exitItem;

    std::variant<ItemHandler, TextHandler> m_handler;

    std::vector<std::unique_ptr<Item>> m_items;
};

class MenuMngr final : public IMenuMngr
{
public:
    MenuMngr() = default;
    ~MenuMngr() = default;

    // IMenuMngr
    std::weak_ptr<IMenu> registerMenu(const std::variant<Menu::ItemHandler, Menu::TextHandler> &handler,
                       bool global) override;
    void destroyMenu(std::weak_ptr<IMenu> menu) override;

    // MenuMngr
    void clearMenus();
    bool ClientCommand(Metamod::Engine::IEdict *pEntity);

private:
    std::vector<std::unique_ptr<Menu>> m_menus;
};
