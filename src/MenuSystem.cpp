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

MenuItem::MenuItem(std::string_view name,
                   MenuItemCallback callback,
                   void *data,
                   NavigationType type) : m_name(name),
                                          m_callback(callback),
                                          m_data(data),
                                          m_type(type)
{}

const char *MenuItem::getName() const
{
    return m_name.data();
}
void MenuItem::setName(const char *name)
{
    setNameCore(name);
}

void *MenuItem::getData() const
{
    return m_data;
}

void MenuItem::setData(void *data)
{
    m_data = data;
}

NavigationType MenuItem::getNavType() const
{
    return m_type;
}

void MenuItem::setCallback(MenuItemCallback func)
{
    m_callback = func;
}

// MenuItem
std::string_view MenuItem::getNameCore() const
{
    return m_name;
}
void MenuItem::setNameCore(std::string_view name)
{
    m_name = name.data();
}

ItemStatus MenuItem::execCallbackCore(Menu *menu,
                                      std::shared_ptr<MenuItem> item,
                                      std::shared_ptr<Player> player) const
{
    ItemStatus result = ItemStatus::Enabled;
    
    /*try
    {
        auto *func = std::get<SourcePawn::IPluginFunction *>(m_callback);
        if(func && func->IsRunnable())
        {
            func->PushCell(static_cast<cell_t>(menu->getId()));
            // item index?
            func->PushCell(static_cast<cell_t>(PACK_ITEM(menu->getId(), menu->getItemIndex(item))));

            func->PushCell(static_cast<cell_t>(player->getIndex()));
            func->Execute(reinterpret_cast<cell_t*>(&result));
        }
    }*/
    result = m_callback(menu, item.get(), player.get());

    return result;
}

Menu::Menu(std::variant<MenuItemHandler, MenuTextHandler> &&handler,
           IMenu::Style style,
           bool global) : m_style(style),
                          m_global(global),
                          m_title(""),
                          m_numberFormat("\\r#num."),
                          m_time(-1),
                          m_itemsPerPage(7),
                          m_keys(0),
                          m_nextItem(std::make_shared<MenuItem>("Next", MenuItemCallback{nullptr}, nullptr, NavigationType::Next)),
                          m_backItem(std::make_shared<MenuItem>("Back", MenuItemCallback{nullptr}, nullptr, NavigationType::Back)),
                          m_exitItem(std::make_shared<MenuItem>("Exit", MenuItemCallback{nullptr}, nullptr, NavigationType::Exit)),
                          m_handler(handler)
{}

void Menu::display(IPlayer *player,
                   int page,
                   int time)
{
    std::shared_ptr<Player> pPlayer = gSPGlobal->getPlayerManagerCore()->getPlayerCore(player->getEdict());
    displayCore(pPlayer, page, time);
}

void Menu::displayCore(std::shared_ptr<Player> player, int page, int time)
{
    const std::unique_ptr<Utils> &utils = gSPGlobal->getUtilsCore();
    char buffer[512];

    if(m_style == IMenu::Style::Item)
    {
        // format and show menu
        int keys = 0;

        // cache this?
        // cache to m_text if all items without callback
        std::stringstream text;

        text << m_title << "\n\n";

        std::size_t start = page * m_itemsPerPage;
        std::size_t hidden = 0;
        
        if(page)
        {
            for(std::size_t j = 0; j < start; j++)
            {
                if(m_items[j]->execCallbackCore(this, m_items[j], player) == ItemStatus::Hide)
                    hidden++;
            }
            start += hidden;

            std::size_t statics = 0;
            for(std::size_t j = 0; j < m_itemsPerPage; j++)
            {
                if(m_staticItems[j])
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
            std::shared_ptr<MenuItem> item = m_staticItems[slot] ? m_staticItems[slot] : m_items[i];

            ret = item->execCallbackCore(this, item, player);

            if(ret == ItemStatus::Hide)
            {
                if(!m_staticItems[slot])
                    ++i;
                continue;
            }

            addItem(ret, slot, item->getNameCore());
            m_slots[slot] = item;

            if(!m_staticItems[slot])
                ++i;
            
            slot++;
        }

        text << "\n";

        while(slot < 7)
        {
            // check for static
            if(m_staticItems[slot])
            {
                std::shared_ptr<MenuItem> item = m_staticItems[slot];
                ret = item->execCallbackCore(this, item, player);

                if(ret != ItemStatus::Hide)
                {
                    addItem(ret, slot, item->getNameCore());
                    m_slots[slot] = item;
                }
            }
            else
            {
                text << "\n";
            }

            slot++;
        }

        // TODO: add custom names for navigation items

        if(m_items.size() - hidden > i)
        {
            addItem(ItemStatus::Enabled, slot, m_nextItem->getNameCore());
            m_slots[slot++] = m_nextItem;
        }
        else
        {
            slot++;
            text << "\n";
        }

        if(page)
        {
            addItem(ItemStatus::Enabled, slot, m_backItem->getNameCore());
            m_slots[slot++] = m_backItem;
        }
        else
        {
            slot++;
            text << "\n";
        }

        addItem(ItemStatus::Enabled, slot, m_exitItem->getNameCore());
        m_slots[slot] = m_exitItem;

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
    UTIL_ShowMenu(player->getEdict(), m_keys, time, buffer, strlen(buffer));
}

bool Menu::getGlobal() const
{
    return m_global;
}

IMenu::Style Menu::getStyle() const
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
    m_itemsPerPage = min(value, static_cast<std::size_t>(10));
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

std::shared_ptr<MenuItem> Menu::keyToItem(int key) const
{
    return m_slots[key].lock();
}

void Menu::appendItem(const char *name,
                      MenuItemCallback callback,
                      void *data)
{
    appendItemCore(name, callback, data);
}

void Menu::appendItemCore(std::string_view name,
                          MenuItemCallback callback,
                          void *data)
{
    _addItem(-1, name, callback, data);
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
                          MenuItemCallback callback,
                          void *data)
{
    if(position >= m_items.size())
        return false;
    
    _addItem(position, name, callback, data);

    return true;
}

bool Menu::setStaticItem(std::size_t position,
                         const char *name,
                         MenuItemCallback callback,
                         void *data)
{
    return setStaticItemCore(position, name, callback, data);
}

bool Menu::setStaticItemCore(std::size_t position,
                             std::string_view name,
                             MenuItemCallback callback,
                             void *data)
{
    if(position >= m_itemsPerPage)
        return false;
    
    m_staticItems[position] = std::make_shared<MenuItem>(name.data(), callback, data, NavigationType::None);

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

IMenuItem *Menu::getItem(std::size_t position) const
{
    return getItemCore(position).get();
}
std::shared_ptr<MenuItem> Menu::getItemCore(std::size_t position) const
{
    if(position >= m_items.size() && position < m_items.size() + MAX_STATIC_ITEMS)
        return m_staticItems[position - m_items.size()];
    else if(position < m_items.size())
        return m_items[position];
    
    return nullptr;
}
int Menu::getItemIndex(std::shared_ptr<MenuItem> item) const
{
    for(std::size_t i = 0; i < MAX_STATIC_ITEMS; i++)
    {
        if(m_staticItems[i] == item)
            return m_items.size() + i;
    }
    for(std::size_t i = 0; i < m_items.size(); i++)
    {
        if(m_items[i] == item)
            return i;
    }
    return -1;
}

void Menu::execTextHandler(std::shared_ptr<Player> player,
                           int key)
{
    /*try
    {
        auto *func = std::get<SourcePawn::IPluginFunction *>(m_handler);
        if(func && func->IsRunnable())
        {
            func->PushCell(static_cast<cell_t>(m_id));
            func->PushCell(static_cast<cell_t>(key));
            func->PushCell(static_cast<cell_t>(player->getIndex()));
            func->Execute(nullptr);
        }
    }*/
    auto func = std::get<MenuTextHandler>(m_handler);
    func(this, key, player.get());
}

void Menu::execItemHandler(std::shared_ptr<Player> player,
                           std::shared_ptr<MenuItem> item)
{
    /*try
    {
        auto *func = std::get<SourcePawn::IPluginFunction *>(m_handler);
        if(func && func->IsRunnable())
        {
            func->PushCell(static_cast<cell_t>(m_id));

            if(item->getNavType() == NavigationType::None)
            {
                func->PushCell(static_cast<cell_t>(PACK_ITEM(m_id, getItemIndex(item))));
            }
            else
            {
                func->PushCell(-static_cast<cell_t>(item->getNavType()));
            }

            func->PushCell(static_cast<cell_t>(player->getIndex()));
            func->Execute(nullptr);
        }
    }*/
    auto func = std::get<MenuItemHandler>(m_handler);
    func(this, item.get(), player.get());
}

void Menu::execExitHandler(std::shared_ptr<Player> player)
{
    execItemHandler(player, m_exitItem);
}

void Menu::_addItem(int position,
                    std::string_view name,
                    MenuItemCallback callback,
                    void *data)
{
    auto item = std::make_shared<MenuItem>(name.data(), callback, data, NavigationType::None);
    if(position == -1)
    {
        m_items.push_back(item);
    }
    else
    {
        m_items.insert(m_items.begin() + position, item);
    }
}

IMenu *MenuMngr::registerMenu(MenuItemHandler handler,
                              bool global)
{
    return registerMenuCore(handler, IMenu::Style::Item, global).get();
}

IMenu *MenuMngr::registerMenu(MenuTextHandler handler,
                              bool global)
{
    return registerMenuCore(handler, IMenu::Style::Text, global).get();
}

void MenuMngr::destroyMenu(IMenu *menu)
{
    for (auto pmenu : m_menus)
    {
        if (pmenu.get() == menu)
        {
            _destroyMenu(pmenu);
            break;
        }
    }
}

void MenuMngr::_destroyMenu(std::shared_ptr<Menu> menu)
{
    // close menu for any player
    const std::unique_ptr<PlayerMngr> &plrMngr = gSPGlobal->getPlayerManagerCore();

    for(unsigned int i = 1; i <= plrMngr->getMaxClients(); i++)
    {
        std::shared_ptr<Player> pPlayer = plrMngr->getPlayerCore(i);
        std::shared_ptr<Menu> pMenu = pPlayer->getMenu().lock();

        if(!pPlayer->isInGame()
            || !pMenu
            || pMenu != menu)
        {
            continue;
        }

        closeMenu(pPlayer);
    }

    auto iter = m_menus.begin();
    while (iter != m_menus.end())
    {
        if (*iter == menu)
        {
            m_menus.erase(iter);
            break;
        }
        ++iter;
    }
}

void MenuMngr::clearMenus()
{
    m_menus.clear();
}

void MenuMngr::displayMenu(std::shared_ptr<Menu> menu,
                           std::shared_ptr<Player> player,
                           int page,
                           int time)
{
    closeMenu(player);

    player->setMenu(menu);
    player->setMenuPage(page);

    menu->displayCore(player, page, time);
}

void MenuMngr::closeMenu(std::shared_ptr<Player> player)
{
    std::shared_ptr<Menu> pMenu = player->getMenu().lock();

    if(!pMenu)
        return;

    player->setMenu(nullptr);

    if(pMenu->getStyle() == IMenu::Style::Item)
        pMenu->execExitHandler(player);
}

META_RES MenuMngr::ClientCommand(edict_t *pEntity)
{
    int pressedKey = std::stoi(CMD_ARGV(1), nullptr, 0) - 1;

    std::shared_ptr<Player> pPlayer = gSPGlobal->getPlayerManagerCore()->getPlayerCore(pEntity);
    std::shared_ptr<Menu> pMenu = pPlayer->getMenu().lock();

    if(!pMenu)
        return MRES_IGNORED;
    
    if(pMenu->getKeys() & (1 << pressedKey))
    {
        pPlayer->setMenu(nullptr);

        if(pMenu->getStyle() == IMenu::Style::Item)
        {
            std::shared_ptr<MenuItem> item = pMenu->keyToItem(pressedKey);

            pMenu->execItemHandler(pPlayer, item);

            if(item->getNavType() == NavigationType::Back)
            {
                displayMenu(pMenu, pPlayer, pPlayer->getMenuPage() - 1, pMenu->getTime());
            }
            else if(item->getNavType() == NavigationType::Next)
            {
                displayMenu(pMenu, pPlayer, pPlayer->getMenuPage() + 1, pMenu->getTime());
            }
            else if(!pMenu->getGlobal())
            {
                destroyMenuCore(pMenu);
            }
        }
        else
        {
            pMenu->execTextHandler(pPlayer, pressedKey);
        }

        return MRES_SUPERCEDE;
    }

    return MRES_IGNORED;
}

void MenuMngr::ClientDisconnected(edict_t *pEntity)
{
    std::shared_ptr<Player> pPlayer = gSPGlobal->getPlayerManagerCore()->getPlayerCore(pEntity);
    closeMenu(pPlayer);
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
