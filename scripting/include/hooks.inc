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
#include <clients>

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


methodmap ClientConnectHookChain
{
    public native void CallNext(Client client, const char[] name, const char[] address, char rejectReason[128]);
    public native void CallOriginal(Client client, const char[] name, const char[] address, char rejectReason[128]);
}

typedef ClientConnectHookCallback = function bool (const ClientConnectHookChain chain, Client client, const char[] name, const char[] address, char rejectReason[128]);

methodmap ClientConnectHook
{
    public native ClientConnectHook(ClientConnectHookCallback callback, HookPriority priority = HookPrio_Default);
    public native void Unhook();
}
