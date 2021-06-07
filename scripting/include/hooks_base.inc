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

#if defined _hooks_base_included
    #endinput
#endif
#define _hooks_base_included

enum HookPriority
{
    HookPrio_UnInterruptable = 255, // Hook will be called before other hooks.
    HookPrio_High = 192,            // Hook will be called before hooks with default priority.
    HookPrio_Default = 128,         // Default hook call priority.
    HookPrio_Medium = 64,           // Hook will be called after hooks with default priority.
    HookPrio_Low = 0,               // Hook will be called after all other hooks.
};

enum CallType
{
    Not_Hookable,
    Hookable
};
