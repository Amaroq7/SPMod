/*
 *  Copyright (C) 2018-2020 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SPMod is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with SPMod.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "ExtMain.hpp"

namespace SPExt::Listener
{
    void Forward(SPMod::IForward *const fwd, int &result, bool &stop);
    void Cvar(const SPMod::ICvar *const cvar, std::string_view old_value, std::string_view new_value);
    void Menu(SPMod::IMenu *const menu, SPMod::IMenu::IItem *const item, SPMod::IPlayer *const player, std::any data);
    void MenuText(SPMod::IMenu *const menu, int key, SPMod::IPlayer *const player, std::any data);
    SPMod::IMenu::IItem::Status MenuItemCallback(SPMod::IMenu *const menu,
                                                 SPMod::IMenu::IItem *const item,
                                                 SPMod::IPlayer *const player,
                                                 std::any data);
    SPMod::IForward::ReturnValue CmdCallback(SPMod::IPlayer *const player, SPMod::ICommand *const cmd, std::any data);
    bool TimerCallback(SPMod::ITimer *const timer, std::any data);
    SPMod::IForward::ReturnValue MessageHookCallback(SPMod::IMessage *const message, std::any cbData);
} // namespace SPExt::Listener
