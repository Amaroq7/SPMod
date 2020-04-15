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

#include "spmod.hpp"

std::int32_t gmsgShowMenu = 0;
std::int32_t gmsgVGUIMenu = 0;

Menu::Item::Item(Menu *menu,
                 std::string_view name,
                 Callback callback,
                 std::any cbData,
                 std::any data,
                 NavigationType type)
    : m_menu(menu), m_name(name), m_callback(callback), m_data(data), m_cbData(cbData), m_type(type)
{
}

std::any Menu::Item::getData() const
{
    return m_data;
}

void Menu::Item::setData(std::any data)
{
    m_data = data;
}

NavigationType Menu::Item::getNavType() const
{
    return m_type;
}

void Menu::Item::setCallback(Callback func, std::any data)
{
    m_callback = func;
    m_cbData = data;
}

// MenuItem
std::string_view Menu::Item::getName() const
{
    return m_name;
}
void Menu::Item::setName(std::string_view name)
{
    m_name = name;
}

Menu::Item::Status Menu::Item::execCallback(IPlayer *player)
{
    return (m_callback ? m_callback(m_menu, this, player, m_cbData) : Status::Enabled);
}

Menu::Menu(const std::variant<ItemHandler, TextHandler> &handler, std::any data, Menu::Style style, bool global)
    : m_style(style), m_global(global), m_numberFormat("\\r#num."), m_time(static_cast<std::uint32_t>(-1)),
      m_itemsPerPage(7), m_keys(0), m_cbData(data),
      m_nextItem(std::make_unique<Item>(this, "Next", Item::Callback(), std::any(), std::any(), NavigationType::Next)),
      m_backItem(std::make_unique<Item>(this, "Back", Item::Callback(), std::any(), std::any(), NavigationType::Back)),
      m_exitItem(std::make_unique<Item>(this, "Exit", Item::Callback(), std::any(), std::any(), NavigationType::Exit)),
      m_handler(handler)
{
}

void Menu::display(IPlayer *player, std::uint32_t page, std::uint32_t time)
{
    Utils *utils = gSPGlobal->getUtils();
    auto playerImpl = dynamic_cast<Player *>(player);

    playerImpl->setMenu(this);
    playerImpl->setMenuPage(page);

    if (m_style == IMenu::Style::Item)
    {
        // format and show menu
        std::uint32_t keys = 0;

        // cache this?
        // cache to m_text if all items without callback
        std::stringstream text;

        text << m_title << "\n\n";

        std::size_t start = page * m_itemsPerPage;
        std::size_t hidden = 0;

        if (page)
        {
            for (std::size_t j = 0; j < start; j++)
            {
                if (m_items[j]->execCallback(player) == Item::Status::Hide)
                    hidden++;
            }
            start += hidden;

            std::size_t statics = 0;
            for (std::size_t j = 0; j < m_itemsPerPage; j++)
            {
                if (m_staticItems[j])
                    statics++;
            }
            start -= statics * page;
        }

        std::size_t slot = 0;

        auto addItem = [&](Item::Status r, std::size_t s, std::string_view n) {
            text << utils->strReplaced(m_numberFormat, "#num", std::to_string(s + 1 == 10 ? 0 : s + 1));

            if (r == Item::Status::Enabled)
            {
                text << " \\w" << n << "\n";
                keys |= (1 << s);
            }
            else
            {
                text << " \\d" << n << "\n";
            }
        };

        auto ret = Item::Status::Enabled;

        std::size_t i = start;
        while (slot < m_itemsPerPage && i < m_items.size())
        {
            const std::unique_ptr<Item> &item = m_staticItems[slot] ? m_staticItems[slot] : m_items[i];

            ret = item->execCallback(player);

            if (ret == Item::Status::Hide)
            {
                if (!m_staticItems[slot])
                    ++i;

                continue;
            }

            addItem(ret, slot, item->getName());
            m_slots[slot] = item.get();

            if (!m_staticItems[slot])
                ++i;

            slot++;
        }

        text << "\n";

        while (slot < 7)
        {
            // check for static
            if (m_staticItems[slot])
            {
                const std::unique_ptr<Item> &item = m_staticItems[slot];
                ret = item->execCallback(player);

                if (ret != Item::Status::Hide)
                {
                    addItem(ret, slot, item->getName());
                    m_slots[slot] = item.get();
                }
            }
            else
            {
                text << "\n";
            }

            slot++;
        }

        // TODO: add custom names for navigation items

        if (m_items.size() - hidden > i)
        {
            addItem(Item::Status::Enabled, slot, m_nextItem->getName());
            m_slots[slot++] = m_nextItem.get();
        }
        else
        {
            slot++;
            text << "\n";
        }

        if (page)
        {
            addItem(Item::Status::Enabled, slot, m_backItem->getName());
            m_slots[slot++] = m_backItem.get();
        }
        else
        {
            slot++;
            text << "\n";
        }

        addItem(Item::Status::Enabled, slot, m_exitItem->getName());
        m_slots[slot] = m_exitItem.get();

        // TODO: add color autodetect (hl don't show colors)
        // TODO: color tags, remove if game mode unsupport it
        m_keys = keys;
        m_text = text.str();
    }

    m_time = time;

    // show
    gSPGlobal->getUtils()->ShowMenu(playerImpl, m_keys, time, m_text);
}

bool Menu::getGlobal() const
{
    return m_global;
}

Menu::Style Menu::getStyle() const
{
    return m_style;
}

void Menu::setText(std::string_view text)
{
    m_text = text;
}

void Menu::setKeys(std::uint32_t keys)
{
    m_keys = keys;
}

void Menu::setTitle(std::string_view text)
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

std::uint32_t Menu::getTime() const
{
    return m_time;
}

std::uint32_t Menu::getKeys() const
{
    return m_keys;
}

Menu::Item *Menu::keyToItem(std::uint32_t key) const
{
    return m_slots.at(key);
}

Menu::Item *Menu::appendItem(std::string_view name, Item::Callback callback, std::any cbData, std::any data)
{
    return _addItem(static_cast<std::uint32_t>(-1), name, callback, cbData, data);
}

Menu::Item *Menu::insertItem(std::size_t position,
                             std::string_view name,
                             Item::Callback callback,
                             std::any cbData,
                             std::any data)
{
    if (position >= m_items.size())
        return nullptr;

    return _addItem(position, name, callback, cbData, data);
}

Menu::Item *Menu::setStaticItem(std::size_t position,
                                std::string_view name,
                                Item::Callback callback,
                                std::any cbData,
                                std::any data)
{
    if (position >= m_itemsPerPage)
        return nullptr;

    m_staticItems[position] = std::make_unique<Item>(this, name, callback, cbData, data, NavigationType::None);
    return m_staticItems[position].get();
}

bool Menu::removeItem(std::size_t position)
{
    if (position >= m_items.size())
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

std::uint32_t Menu::getItemIndex(const IItem *item) const
{
    for (std::size_t i = 0; i < MAX_STATIC_ITEMS; i++)
    {
        if (m_staticItems[i].get() == item)
            return m_items.size() + i;
    }

    for (std::size_t i = 0; i < m_items.size(); i++)
    {
        if (m_items[i].get() == item)
            return i;
    }
    return static_cast<std::uint32_t>(-1);
}

Menu::Item *Menu::getItem(std::size_t position) const
{
    if (position >= m_items.size() && position < m_items.size() + MAX_STATIC_ITEMS)
        return m_staticItems[position - m_items.size()].get();
    else if (position < m_items.size())
        return m_items[position].get();

    return nullptr;
}

void Menu::execTextHandler(Player *player, std::uint32_t key)
{
    auto func = std::get<TextHandler>(m_handler);
    func(this, key, player, m_cbData);
}

void Menu::execItemHandler(Player *player, Menu::Item *item)
{
    auto func = std::get<ItemHandler>(m_handler);
    func(this, item, player, m_cbData);
}

std::any Menu::getCallbackData() const
{
    return m_cbData;
}

void Menu::execExitHandler(Player *player)
{
    execItemHandler(player, m_exitItem.get());
}

Menu::Item *Menu::_addItem(std::uint32_t position,
                           std::string_view name,
                           Item::Callback callback,
                           std::any cbData,
                           std::any data)
{
    if (position == static_cast<std::uint32_t>(-1))
    {
        return m_items.emplace_back(std::make_unique<Item>(this, name, callback, cbData, data, NavigationType::None))
            .get();
    }
    else
    {
        return (*m_items.insert(m_items.begin() + position,
                                std::make_unique<Item>(this, name, callback, cbData, data, NavigationType::None)))
            .get();
    }
}

Menu *MenuMngr::registerMenu(const std::variant<Menu::ItemHandler, Menu::TextHandler> &handler,
                             std::any data,
                             bool global)
{
    return std::visit(
        [=](auto &&arg) {
            using T = std::decay_t<decltype(arg)>;

            if constexpr (std::is_same_v<T, Menu::ItemHandler>)
            {
                return m_menus.emplace_back(std::make_unique<Menu>(handler, data, IMenu::Style::Item, global)).get();
            }
            else if constexpr (std::is_same_v<T, Menu::TextHandler>)
            {
                return m_menus.emplace_back(std::make_unique<Menu>(handler, data, IMenu::Style::Text, global)).get();
            }
        },
        handler);
}

void MenuMngr::destroyMenu(const IMenu *menu)
{
    // close menu for any player
    PlayerMngr *plrMngr = gSPGlobal->getPlayerManager();

    for (std::uint32_t i = 1; i <= plrMngr->getMaxClients(); i++)
    {
        Player *pPlayer = plrMngr->getPlayer(i);
        Menu *pMenu = pPlayer->getMenu();

        if (!pPlayer->isInGame() || !pMenu || pMenu != menu)
        {
            continue;
        }

        pPlayer->closeMenu();
    }

    auto iter = m_menus.begin();
    while (iter != m_menus.end())
    {
        if ((*iter).get() == menu)
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

META_RES MenuMngr::ClientCommand(edict_t *pEntity)
{
    std::string_view pressedKeyText(CMD_ARGV(1));
    std::uint32_t pressedKey;
    if (const auto &[ptr, ec] =
            std::from_chars(pressedKeyText.data(), pressedKeyText.data() + pressedKeyText.length(), pressedKey);
        ec != std::errc())
    {
        return MRES_IGNORED;
    }

    pressedKey -= 1;

    Player *pPlayer = gSPGlobal->getPlayerManager()->getPlayer(pEntity);
    Menu *pMenu = pPlayer->getMenu();

    if (pMenu && pMenu->getKeys() & (1 << pressedKey))
    {
        pPlayer->setMenu(nullptr);

        if (pMenu->getStyle() == IMenu::Style::Item)
        {
            Menu::Item *item = pMenu->keyToItem(pressedKey);

            pMenu->execItemHandler(pPlayer, item);

            if (item->getNavType() == NavigationType::Back)
            {
                pMenu->display(pPlayer, pPlayer->getMenuPage() - 1, pMenu->getTime());
            }
            else if (item->getNavType() == NavigationType::Next)
            {
                pMenu->display(pPlayer, pPlayer->getMenuPage() + 1, pMenu->getTime());
            }
            else if (!pMenu->getGlobal())
            {
                destroyMenu(pMenu);
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
