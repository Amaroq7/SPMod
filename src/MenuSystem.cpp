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

#include "spmod.hpp"

int gmsgShowMenu = 0;
int gmsgVGUIMenu = 0;

Menu::MenuItem::MenuItem(std::string n,
                 std::variant<SourcePawn::IPluginFunction *, MenuItemCallback> &&c,
                 std::variant<cell_t, void *> d) : name(n),
                                                   callback(c),
                                                   data(d)
{}
ItemStatus Menu::MenuItem::execCallback(Menu *menu, std::size_t i, int player)
{
    ItemStatus result = ItemStatus::Enabled;
    
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

Menu::Menu(std::size_t id,
           std::variant<SourcePawn::IPluginFunction *, MenuHandler> &&handler,
           MenuStyle style,
           bool global) : m_id(id),
                          m_style(style),
                          m_global(global),
                          m_title(""),
                          m_numberFormat("\\r#num."),
                          m_time(-1),
                          m_itemsPerPage(7),
                          m_keys(0),
                          m_handler(handler)
{}

void Menu::display(int player, int page, int time)
{
    const std::unique_ptr<Utils> &utils = gSPGlobal->getUtilsCore();
    char buffer[512];

    if(m_style == MenuStyle::Item)
    {
        // format and show menu
        int keys = 0;

        // cache this?
        std::stringstream text;

        text << m_title << "\n\n";

        std::size_t start = page * m_itemsPerPage;
        std::size_t hidden = 0;
        
        if(page)
        {
            for(std::size_t j = 0; j < start; j++)
            {
                if(m_items[j].execCallback(this, j, player) == ItemStatus::Hide)
                    hidden++;
            }
            start += hidden;

            std::size_t statics = 0;
            for(std::size_t j = 0; j < m_itemsPerPage; j++)
            {
                if(m_staticSlots[j])
                    statics++;
            }
            start -= statics * page;
        }

        std::size_t slot = 0;

        auto addItem = [&](ItemStatus r, std::size_t s, std::string_view n)
        {
            text << utils->strReplacedCore(m_numberFormat, "#num", std::to_string(s + 1 == 10 ? 0 : s + 1));

            if(r == ItemStatus::Enabled)
            {
                text << " \\w" << n << "\n";
                keys |= (1 << s);
            }
            else
            {
                text << " \\d" << n << "\n";
            }
        };

        ItemStatus ret = ItemStatus::Enabled;

        std::size_t i = start;
        while(slot < m_itemsPerPage && i < m_items.size())
        {
            MenuItem &item = m_staticSlots[slot] ? *m_staticSlots[slot] : m_items[i];

            ret = item.execCallback(this, m_staticSlots[slot] ? m_items.size() + slot : i, player);

            if(ret == ItemStatus::Hide)
            {
                if(!m_staticSlots[slot])
                    ++i;
                continue;
            }

            addItem(ret, slot, item.name);
            m_slots[slot] = m_staticSlots[slot] ? m_items.size() + slot : i;

            if(!m_staticSlots[slot])
                ++i;
            
            slot++;
        }

        text << "\n";

        while(slot < 7)
        {
            // check for static
            if(m_staticSlots[slot])
            {
                MenuItem &item = *m_staticSlots[slot];
                ret = item.execCallback(this, m_items.size() + slot, player);

                if(ret != ItemStatus::Hide)
                {
                    addItem(ret, slot, item.name);
                    m_slots[slot] = m_items.size() + slot;
                }
            }
            else
            {
                text << "\n";
            }

            slot++;
        }

        if(m_items.size() - hidden > i)
        {
            addItem(ItemStatus::Enabled, slot, "Next");
            m_slots[slot++] = MENU_NEXT;
        }
        else
        {
            slot++;
            text << "\n";
        }

        if(page)
        {
            addItem(ItemStatus::Enabled, slot, "Back");
            m_slots[slot++] = MENU_BACK;
        }
        else
        {
            slot++;
            text << "\n";
        }

        addItem(ItemStatus::Enabled, slot, "Exit");
        m_slots[slot] = MENU_EXIT;

        // TODO: add color autodetect (hl don't show colors)
        // TODO: color tags, remove if game mode unsupport it

        utils->strCopyCore(buffer, sizeof(buffer), text.str());

        m_keys = keys;
    }
    else
    {
        // text style
        utils->strCopyCore(buffer, sizeof(buffer), m_text);
    }

    m_time = time;

    //show
    UTIL_ShowMenu(INDEXENT(player), m_keys, time, buffer, strlen(buffer));
}

bool Menu::getGlobal() const
{
    return m_global;
}

MenuStyle Menu::getStyle() const
{
    return m_style;
}

void Menu::setText(const char *text)
{
    setTextCore(text);
}
void Menu::setTextCore(std::string_view text)
{
    m_text = text.data();
}
void Menu::setKeys(int keys)
{
    m_keys = keys;
}

void Menu::setTitle(const char *text)
{
    setTitleCore(text);
}

void Menu::setTitleCore(std::string_view text)
{
    m_title = text;
}
void Menu::setItemsPerPage(std::size_t value)
{
    m_itemsPerPage = value;
}
std::size_t Menu::getItemsPerPage() const
{
    return m_itemsPerPage;
}

void Menu::setNumberFormat(std::string_view format)
{
    m_numberFormat = format;
}

int Menu::getTime() const
{
    return m_time;
}

int Menu::getKeys() const
{
    return m_keys;
}

int Menu::keyToSlot(int key) const
{
    return m_slots[key];
}

void Menu::appendItem(const char *name,
                      MenuItemCallback callback,
                      void *data)
{
    appendItemCore(name, callback, data);
}

void Menu::appendItemCore(std::string_view name,
                          std::variant<SourcePawn::IPluginFunction *, MenuItemCallback> &&callback,
                          std::variant<cell_t, void *> &&data)
{
    _addItem(-1, name, std::move(callback), std::move(data));
}

bool Menu::insertItem(std::size_t position,
                      const char *name,
                      MenuItemCallback callback,
                      void *data)
{
    return insertItemCore(position, name, callback, data);
}

bool Menu::insertItemCore(std::size_t position,
                          std::string_view name,
                          std::variant<SourcePawn::IPluginFunction *, MenuItemCallback> &&callback,
                          std::variant<cell_t, void *> &&data)
{
    if(position >= m_items.size())
        return false;
    
    _addItem(position, name, std::move(callback), std::move(data));

    return true;
}

bool Menu::setStaticItem(std::size_t position,
                   std::string_view name,
                   std::variant<SourcePawn::IPluginFunction *, MenuItemCallback> &&callback,
                   std::variant<cell_t, void *> &&data)
{
    if(position >= m_itemsPerPage)
        return false;
    
    m_staticSlots[position] = std::make_unique<MenuItem>(name.data(), std::move(callback), std::move(data));

    return true;
}

bool Menu::removeItem(std::size_t position)
{
    if(position >= m_items.size())
        return false;
    
    m_items.erase(m_items.begin() + position);

    return true;
}
void Menu::removeAllItems()
{
    m_items.clear();
}

std::size_t Menu::getItems() const
{
    return m_items.size();
}

bool Menu::setItemName(std::size_t item,
                       std::string_view name)
{
    if(item >= m_items.size() + 10)
        return false;
    
    if(item >= m_items.size())
        m_staticSlots[item - m_items.size()]->name = name.data();
    else
        m_items[item].name = name.data();

    return true;
}

std::string_view Menu::getItemName(std::size_t item) const
{
    if(item >= m_items.size() + 10)
        return "";

    if(item >= m_items.size())
        return m_staticSlots[item - m_items.size()]->name;
    else
        return m_items[item].name;
}

void Menu::setItemData(std::size_t item,
                       std::variant<cell_t, void *> &&data)
{
    if(item >= m_items.size())
        m_staticSlots[item - m_items.size()]->data = data;
    else
        m_items[item].data = data;
}

cell_t Menu::getItemData(std::size_t item)
{
    try
    {
        if(item >= m_items.size())
            return std::get<cell_t>(m_staticSlots[item - m_items.size()]->data);
        else 
            return std::get<cell_t>(m_items[item].data);
    }
    catch (const std::bad_variant_access &e [[maybe_unused]])
    {
        return 0;
    }
}

void Menu::setHandler(MenuHandler func)
{
    setHandlerCore(func);
}

void Menu::setHandlerCore(std::variant<SourcePawn::IPluginFunction *, MenuHandler> &&func)
{
    m_handler = func;
}

void Menu::execHandler(int player,
                       int item)
{
    try
    {
        auto *func = std::get<SourcePawn::IPluginFunction *>(m_handler);
        if(func && func->IsRunnable())
        {
            
            func->PushCell(static_cast<cell_t>(m_id));

            if(m_style == MenuStyle::Item)
            {
                cell_t packedItem = item >= 0 ? PACK_ITEM(m_id, item) : item;
                func->PushCell(static_cast<cell_t>(packedItem));
            }
            else
            {
                func->PushCell(static_cast<cell_t>(item));
            }
            
            func->PushCell(static_cast<cell_t>(player));
            func->Execute(nullptr);
        }
    }
    catch (const std::bad_variant_access &e [[maybe_unused]])
    {
        auto func = std::get<MenuHandler>(m_handler);
        func(this, item, player);
    }
}

std::size_t Menu::getId() const
{
    return m_id;
}

void Menu::_addItem(int position,
                    std::string_view name,
                    std::variant<SourcePawn::IPluginFunction *, MenuItemCallback> &&callback,
                    std::variant<cell_t, void *> &&data)
{
    MenuItem item(name.data(), std::move(callback), std::move(data));
    if(position == -1)
    {
        m_items.push_back(item);
    }
    else
    {
        m_items.insert(m_items.begin() + position, item);
    }
}

MenuMngr::MenuMngr()
{
    for(int i = 1; i < 33; i++)
    {
        m_playerMenu[i] = -1;
        m_playerPage[i] = 0;
    }
}

IMenu *MenuMngr::registerMenu(MenuHandler handler,
                                 MenuStyle style,
                                 bool global)
{
    return registerMenuCore(handler, style, global).get();
}
IMenu *MenuMngr::registerMenu(SourcePawn::IPluginFunction *func,
                                 MenuStyle style,
                                 bool global)
{
    return registerMenuCore(func, style, global).get();
}

IMenu *MenuMngr::findMenu(std::size_t mid) const
{
    return findMenuCore(mid).get();
}

void MenuMngr::destroyMenu(std::size_t index)
{
    // close menu for any player
    for(int i = 1; i < 33; i++)
    {
        if(m_playerMenu[i] == static_cast<int>(index))
            closeMenu(i);
    }

    auto iter = m_menus.begin();
    while (iter != m_menus.end())
    {
        if ((*iter)->getId() == index)
        {
            m_menus.erase(iter);
            break;
        }
        ++iter;
    }
}

std::shared_ptr<Menu> MenuMngr::findMenuCore(std::size_t index) const
{
    for(auto menu : m_menus)
    {
        if(menu->getId() == index)
            return menu;
    }
    return nullptr;
}

void MenuMngr::clearMenus()
{
    m_menus.clear();
    m_mid = 0;
}

void MenuMngr::displayMenu(std::shared_ptr<Menu> menu, int player, int page, int time)
{
    closeMenu(player);

    m_playerMenu[player] = menu->getId();
    m_playerPage[player] = page;

    menu->display(player, page, time);
}

void MenuMngr::closeMenu(int player)
{
    if(m_playerMenu[player] == -1)
        return;
    
    std::shared_ptr<Menu> pMenu = findMenuCore(m_playerMenu[player]);

    m_playerMenu[player] = -1;

    if(pMenu->getStyle() == MenuStyle::Item)
        pMenu->execHandler(player, MENU_EXIT);
}

META_RES MenuMngr::ClientCommand(edict_t *pEntity)
{
    int pressedKey = std::stoi(CMD_ARGV(1), nullptr, 0) - 1;
    int player = ENTINDEX(pEntity);

    if(m_playerMenu[player] == -1)
        return MRES_IGNORED;
    
    std::shared_ptr<Menu> pMenu = findMenuCore(m_playerMenu[player]);
    
    if(pMenu->getKeys() & (1 << pressedKey))
    {
        m_playerMenu[player] = -1;

        if(pMenu->getStyle() == MenuStyle::Item)
        {
            int slot = pMenu->keyToSlot(pressedKey);

            pMenu->execHandler(player, slot);

            if(slot == MENU_BACK)
            {
                displayMenu(pMenu, player, m_playerPage[player] - 1, pMenu->getTime());
            }
            else if(slot == MENU_NEXT)
            {
                displayMenu(pMenu, player, m_playerPage[player] + 1, pMenu->getTime());
            }
            else if(!pMenu->getGlobal())
            {
                destroyMenu(pMenu->getId());
            }
        }
        else
        {
            pMenu->execHandler(player, pressedKey);
        }

        return MRES_SUPERCEDE;
    }

    return MRES_IGNORED;
}

void MenuMngr::ClientDisconnected(edict_t *pEntity)
{
    closeMenu(ENTINDEX(pEntity));
}

// TODO: move to util.cpp or same
/* warning - don't pass here const string */
void UTIL_ShowMenu(edict_t* pEdict, int slots, int time, char *menu, int mlen)
{
    char *n = menu;
    char c = 0;
    int a;

    if (!gmsgShowMenu)
        return; // some games don't support ShowMenu (Firearms)

    do
    {
        a = mlen;
        if (a > 175) a = 175;
        mlen -= a;
        c = *(n+=a);
        *n = 0;
        
        MESSAGE_BEGIN(MSG_ONE, gmsgShowMenu, nullptr, pEdict);
        WRITE_SHORT(slots);
        WRITE_CHAR(time);
        WRITE_BYTE(c ? true : false);
        WRITE_STRING(menu);
        MESSAGE_END();
        *n = c;
        menu = n;
    }
    while (*n);
}
