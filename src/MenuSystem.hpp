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
std::string replace(const std::string& str, const std::string& from, const std::string& to);

#define PACK_ITEM(menuid, itemid) (menuid << 16 | itemid)
#define UNPACK_ITEM(index, menuid, itemid) menuid = index >> 16; itemid = index & 0xFFFF

class Menu: public IMenu
{
public:
    struct MenuItem
    {
        MenuItem(std::string n,
                 std::variant<SourcePawn::IPluginFunction *, MenuItemCallback> &&c,
                 std::variant<cell_t, void *> d) : name(n),
                                                   callback(c),
                                                   data(d)
        {}
        ItemStatus execCallback(Menu *menu, std::size_t i, int player)
        {
            ItemStatus result = ItemEnabled;
            
            try
            {
                auto *func = std::get<SourcePawn::IPluginFunction *>(callback);
                if(func && func->IsRunnable())
                {
                    func->PushCell(static_cast<cell_t>(menu->getId()));
                    func->PushCell(static_cast<cell_t>(PACK_ITEM(menu->getId(), i)));
                    func->PushCell(static_cast<cell_t>(player));
                    func->Execute(reinterpret_cast<cell_t*>(&result));
                }
            }
            catch (const std::bad_variant_access &e [[maybe_unused]])
            {
                auto func = std::get<MenuItemCallback>(callback);
                if(func)
                {
                    result = func(menu, i, player);
                }
            }

            return result;
        }
        std::string name;
        std::variant<SourcePawn::IPluginFunction *, MenuItemCallback> callback;
        std::variant<cell_t, void *> data;
    };

public:
    Menu(std::size_t id,
         std::variant<SourcePawn::IPluginFunction *, MenuHandler> &&handler,
         MenuStyle style,
         bool global);

    ~Menu() {}

    void display(int player,
                 int page,
                 int time) override;

    bool getGlobal() const override;
    MenuStyle getStyle() const;

    void setText(const char *text) override;
    void setTextCore(std::string_view text);
    void setKeys(int keys) override;

    void setTitle(const char *text) override;
    void setTitleCore(std::string_view text);

    void setItemsPerPage(std::size_t value) override;
    std::size_t getItemsPerPage() const override;

    void setNumberFormat(std::string_view format);

    int getTime() const override;
    int getKeys() const override;
    int keyToSlot(int key) const override;

    void appendItem(const char *name,
                    MenuItemCallback callback,
                    void *data) override;

    void appendItemCore(std::string_view name,
                        std::variant<SourcePawn::IPluginFunction *, MenuItemCallback> &&callback,
                        std::variant<cell_t, void *> &&data);

    bool insertItem(std::size_t position,
                    const char *name,
                    MenuItemCallback callback,
                    void *data) override;
    
    bool insertItemCore(std::size_t position,
                        std::string_view name,
                        std::variant<SourcePawn::IPluginFunction *, MenuItemCallback> &&callback,
                        std::variant<cell_t, void *> &&data);

    bool setStaticItem(std::size_t positon,
                       std::string_view name,
                       std::variant<SourcePawn::IPluginFunction *, MenuItemCallback> &&callback,
                       std::variant<cell_t, void *> &&data);

    bool removeItem(std::size_t position) override;
    void removeAllItems() override;

    std::size_t getItems() const override;

    bool setItemName(std::size_t item,
                     std::string_view name);
    
    std::string_view getItemName(std::size_t item) const;

    void setItemData(std::size_t item,
                     std::variant<cell_t, void *> &&data);
    cell_t getItemData(std::size_t item);

    void setHandler(MenuHandler func) override;
    void setHandlerCore(std::variant<SourcePawn::IPluginFunction *, MenuHandler> &&func);

    void execHandlerItem(int player, int item) override;
    void execHandlerText(int player, int key) override;

    std::size_t getId() const;
private:
    void _addItem(int position,
                  std::string_view name,
                  std::variant<SourcePawn::IPluginFunction *, MenuItemCallback> &&callback,
                  std::variant<cell_t, void *> &&data);
private:
    std::size_t m_id;
    MenuStyle m_style;
    bool m_global;
    std::string m_text;
    std::string m_title;
    std::string m_numberFormat;
    int m_time;
    std::size_t m_itemsPerPage;
    int m_keys;
    int m_slots[10];
    std::unique_ptr<MenuItem> m_staticSlots[10];

    std::variant<SourcePawn::IPluginFunction *, MenuHandler> m_handler;

    std::vector<MenuItem> m_items;
};

class MenuMngr : public IMenuMngr
{
public:
    MenuMngr();
    ~MenuMngr() = default;

    IMenu *registerMenu(MenuHandler handler,
                        MenuStyle style,
                        bool global) override;
    IMenu *registerMenu(SourcePawn::IPluginFunction *func,
                        MenuStyle style,
                        bool global) override;
    IMenu *findMenu(std::size_t mid) const override;

    template<typename ...Args>
    std::shared_ptr<Menu> registerMenuCore(Args... args)
    {
        std::shared_ptr<Menu> menu = std::make_shared<Menu>(m_mid++, std::forward<Args>(args)...);
        m_menus.push_back(menu);
        return menu;
    }
    std::shared_ptr<Menu> findMenuCore(std::size_t index) const;

    void destroyMenu(std::size_t index);
    void clearMenus();

    void displayMenu(std::shared_ptr<Menu> menu, int player, int page, int time);
    void closeMenu(int player);

    META_RES ClientCommand(edict_t *pEntity);
    void ClientDisconnected(edict_t *pEntity);
private:
    std::size_t m_mid = 0;
    std::vector<std::shared_ptr<Menu>> m_menus;

    int m_playerMenu[33];
    int m_playerPage[33];
};
