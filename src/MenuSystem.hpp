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

#define PACK_ITEM(menuid, itemid) (menuid << 16 | itemid)
#define UNPACK_ITEM(index, menuid, itemid) menuid = index >> 16; itemid = index & 0xFFFF

constexpr unsigned int MAX_STATIC_ITEMS = 10U;

class Menu;
class Player;

class MenuItem final : public IMenuItem
{
public:
    MenuItem(std::string_view name,
             MenuItemCallback callback,
             void *data,
             NavigationType type);
    
    ~MenuItem() = default;

    // IMenuItem
    const char *getName() const override;
    void setName(const char *name) override;

    void *getData() const override;
    void setData(void *data) override;

    NavigationType getNavType() const override;

    void setCallback(MenuItemCallback func) override;
    
    // MenuItem
    std::string_view getNameCore() const;
    void setNameCore(std::string_view name);

    ItemStatus execCallbackCore(Menu *menu,
                                std::shared_ptr<MenuItem> item,
                                std::shared_ptr<Player> player) const;

private:
    std::string m_name;
    MenuItemCallback m_callback;
    void *m_data;
    NavigationType m_type;
};

class Menu final : public IMenu
{
public:
    Menu(std::variant<MenuItemHandler, MenuTextHandler> &&handler,
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

    void appendItem(const char *name,
                    MenuItemCallback callback,
                    void *data) override;

    bool insertItem(std::size_t position,
                    const char *name,
                    MenuItemCallback callback,
                    void *data) override;
    
    bool setStaticItem(std::size_t position,
                       const char *name,
                       MenuItemCallback callback,
                       void *data) override;
    
    bool removeItem(std::size_t position) override;
    void removeAllItems() override;

    IMenuItem *getItem(std::size_t position) const override;
    
    std::size_t getItems() const override;

    // Menu
    void displayCore(std::shared_ptr<Player> player,
                     int page,
                     int time);
    
    std::shared_ptr<MenuItem> keyToItem(int key) const;

    void setTextCore(std::string_view text);

    void appendItemCore(std::string_view name,
                        MenuItemCallback callback,
                        void *data);

    bool insertItemCore(std::size_t position,
                        std::string_view name,
                        MenuItemCallback callback,
                        void *data);

    bool setStaticItemCore(std::size_t position,
                           std::string_view name,
                           MenuItemCallback callback,
                           void *data);
    
    std::shared_ptr<MenuItem> getItemCore(std::size_t position) const;
    int getItemIndex(std::shared_ptr<MenuItem> menu) const;

    void setNumberFormat(std::string_view format);

    void execTextHandler(std::shared_ptr<Player> player,
                         int key);
    
    void execItemHandler(std::shared_ptr<Player> player,
                         std::shared_ptr<MenuItem> item);
    
    void execExitHandler(std::shared_ptr<Player> player);

private:
    void _addItem(int position,
                  std::string_view name,
                  MenuItemCallback callback,
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

    std::array<std::weak_ptr<MenuItem>, 10> m_slots;
    std::array<std::shared_ptr<MenuItem>, MAX_STATIC_ITEMS> m_staticItems;

    std::shared_ptr<MenuItem> m_nextItem;
    std::shared_ptr<MenuItem> m_backItem;
    std::shared_ptr<MenuItem> m_exitItem;

    std::variant<MenuItemHandler, MenuTextHandler> m_handler;

    std::vector<std::shared_ptr<MenuItem>> m_items;
};

class MenuMngr final : public IMenuMngr
{
public:
    MenuMngr() = default;
    ~MenuMngr() = default;

    // IMenuMngr
    IMenu *registerMenu(MenuItemHandler handler,
                        bool global) override;
    IMenu *registerMenu(MenuTextHandler handler,
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
    void _destroyMenu(std::shared_ptr<Menu> menu);

private:
    std::vector<std::shared_ptr<Menu>> m_menus;
};
