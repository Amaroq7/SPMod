/*
 *  Copyright (C) 2021 SPMod Development Team
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

#if defined _hooks_included
    #endinput
#endif
#define _hooks_included

#include <hooks_base>
#include <clients_hooks>

methodmap GameInitHookChain
{
    public native void CallNext();
    public native void CallOriginal();
}

typedef GameInitHookCallback = function void (const GameInitHookChain chain);

methodmap GameInitHook
{
    public native GameInitHook(GameInitHookCallback callback, HookPriority priority = HookPrio_Default);
    public native void Unhook();
}

methodmap ChangeLevelHookChain
{
    public native void CallNext(const char[] map, const char[] map2);
    public native void CallOriginal(const char[] map, const char[] map2);
}

/*
 * @brief Called when map is going to change.
 *
 * @param map        Map name.
 * @param map2       Map name.
 *
 * @noreturn
 */
typedef ChangeLevelHookCallback = function void (const ChangeLevelHookChain chain, const char[] map, const char[] map2);

methodmap ChangeLevelHook
{
    public native ChangeLevelHook(ChangeLevelHookCallback callback, HookPriority priority = HookPrio_Default);
    public native void Unhook();
}