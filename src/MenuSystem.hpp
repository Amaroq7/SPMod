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

#include "spmod.hpp"

void UTIL_ShowMenu(edict_t* pEdict, int slots, int time, char *menu, int mlen);

constexpr unsigned int MAX_STATIC_ITEMS = 10U;

class Player;

class Menu final : public IMenu
{
public:
    class Item final : public IMenu::IItem
    {
    public:
        Item(std::string_view name,
             Callback callback,
             void *data,
             NavigationType type);
        
        ~Item() = default;

        // IItem
        const char *getName() const override;
        void setName(const char *name) override;

        void *getData() const override;
        void setData(void *data) override;

        NavigationType getNavType() const override;

        void setCallback(IMenu::IItem::Callback func) override;
        
        // Item
        std::string_view getNameCore() const;
        void setNameCore(std::string_view name);

        Item::Status execCallbackCore(Menu *menu,
                                      std::shared_ptr<Item> item,
                                      std::shared_ptr<Player> player) const;

    private:
        std::string m_name;
        Callback m_callback;
        void *m_data;
        NavigationType m_type;
    };

    Menu(std::variant<ItemHandler, TextHandler> &&handler,
         IMenu::Style style,
         bool global);

    ~Menu() = default;

    // IMenu
    void display(IPlayer *player,
                 int page,
                 int time) override;

    bool getGlobal() const override;
    IMenu::Style getStyle() const override;

    void setText(const char *text) override;
    
    void setKeys(int keys) override;

    void setTitle(const char *text) override;
    void setTitleCore(std::string_view text);

    void setItemsPerPage(std::size_t value) override;
    std::size_t getItemsPerPage() const override;

    int getTime() const override;
    int getKeys() const override;

    IItem *appendItem(const char *name,
                      Item::Callback callback,
                      void *data) override;

    IItem *insertItem(std::size_t position,
                      const char *name,
                      Item::Callback callback,
                      void *data) override;

    IItem *setStaticItem(std::size_t position,
                         const char *name,
                         Item::Callback callback,
                         void *data) override;
    
    bool removeItem(std::size_t position) override;
    void removeAllItems() override;

    IItem *getItem(std::size_t position) const override;
    
    std::size_t getItems() const override;

    // Menu
    void displayCore(std::shared_ptr<Player> player,
                     int page,
                     int time);
    
    std::shared_ptr<Item> keyToItem(int key) const;

    void setTextCore(std::string_view text);

    std::shared_ptr<Item> appendItemCore(std::string_view name,
                                         Item::Callback callback,
                                         void *data);

    std::shared_ptr<Item> insertItemCore(std::size_t position,
                                         std::string_view name,
                                         Item::Callback callback,
                                         void *data);

    std::shared_ptr<Item> setStaticItemCore(std::size_t position,
                                            std::string_view name,
                                            Item::Callback callback,
                                            void *data);
    
    std::shared_ptr<Item> getItemCore(std::size_t position) const;
    int getItemIndex(std::shared_ptr<Item> menu) const;

    void setNumberFormat(std::string_view format);

    void execTextHandler(std::shared_ptr<Player> player,
                         int key);
    
    void execItemHandler(std::shared_ptr<Player> player,
                         std::shared_ptr<Item> item);
    
    void execExitHandler(std::shared_ptr<Player> player);

private:
    std::shared_ptr<Item> _addItem(int position,
                                   std::string_view name,
                                   Item::Callback callback,
                                   void *data);
private:
    IMenu::Style m_style;
    bool m_global;
    std::string m_text;
    std::string m_title;
    std::string m_numberFormat;
    int m_time;
    std::size_t m_itemsPerPage;
    int m_keys;

    std::array<std::weak_ptr<Item>, 10> m_slots;
    std::array<std::shared_ptr<Item>, MAX_STATIC_ITEMS> m_staticItems;

    std::shared_ptr<Item> m_nextItem;
    std::shared_ptr<Item> m_backItem;
    std::shared_ptr<Item> m_exitItem;

    std::variant<ItemHandler, TextHandler> m_handler;

    std::vector<std::shared_ptr<Item>> m_items;
};

class MenuMngr final : public IMenuMngr
{
public:
    MenuMngr() = default;
    ~MenuMngr() = default;

    // IMenuMngr
    IMenu *registerMenu(IMenu::ItemHandler handler,
                        bool global) override;
    IMenu *registerMenu(IMenu::TextHandler handler,
                        bool global) override;

    void destroyMenu(IMenu *menu) override;

    // MenuMngr
    template<typename ...Args>
    std::shared_ptr<Menu> registerMenuCore(Args... args)
    {
        return m_menus.emplace_back(std::make_shared<Menu>(std::forward<Args>(args)...));
    }

    void clearMenus();
    void destroyMenuCore(std::shared_ptr<Menu> menu);

    void displayMenu(std::shared_ptr<Menu> menu, std::shared_ptr<Player> player, int page, int time);
    void closeMenu(std::shared_ptr<Player> player);

    META_RES ClientCommand(edict_t *pEntity);
    void ClientDisconnected(edict_t *pEntity);

private:
    std::vector<std::shared_ptr<Menu>> m_menus;
};
