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

methodmap ClientConnectHookChain
{
    public native bool CallNext(Client client, const char[] name, const char[] address, char rejectReason[128]);
    public native bool CallOriginal(Client client, const char[] name, const char[] address, char rejectReason[128]);
}

/*
 * @brief Called when a client connects to the server.
 *
 * @param client        Client id.
 * @param name          Name of the client.
 * @param ip            IP address of the client.
 * @param reason        Reason that will be displayed to the client when they get rejected.
 *
 * @return              True to let client join or false to reject them.
 */
typedef ClientConnectHookCallback = function bool (const ClientConnectHookChain chain, Client client, const char[] name, const char[] address, char rejectReason[128]);

methodmap ClientConnectHook
{
    public native ClientConnectHook(ClientConnectHookCallback callback, HookPriority priority = HookPrio_Default);
    public native void Unhook();
}

methodmap ClientDropHookChain
{
    public native void CallNext(Client client, bool crash, const char[] reason);
    public native void CallOriginal(Client client, bool crash, const char[] reason);
}

/*
 * @brief Called when a client drops from the server.
 *
 * @param client        Client id.
 * @param crash         True if client crashed, false otherwise.
 * @param reason        Reason why the client has been disconnected.
 *
 * @noreturn
 */
typedef ClientDropHookCallback = function void (const ClientDropHookChain chain, Client client, bool crash, const char[] reason);

methodmap ClientDropHook
{
    public native ClientDropHook(ClientDropHookCallback callback, HookPriority priority = HookPrio_Default);
    public native void Unhook();
}

methodmap ClientPutinServerHookChain
{
    public native void CallNext(Client client);
    public native void CallOriginal(Client client);
}

/*
 * @brief Called when a client enters the game.
 *
 * @param client        Client id.
 *
 * @noreturn
 */
typedef ClientPutinServerHookCallback = function void (const ClientPutinServerHookChain chain, Client client);

methodmap ClientPutinServerHook
{
    public native ClientPutinServerHook(ClientPutinServerHookCallback callback, HookPriority priority = HookPrio_Default);
    public native void Unhook();
}

methodmap ClientInfoChangedHookChain
{
    public native void CallNext(Client client, InfoBuffer infoBuffer);
    public native void CallOriginal(Client client, InfoBuffer infoBuffer);
}

/*
 * @brief Called when client info is changed.
 *
 * @param client        Client id.
 * @param infoBuffer    Client info buffer.
 *
 * @noreturn
 */
typedef ClientInfoChangedHookCallback = function void (const ClientPutinServerHookChain chain, Client client, InfoBuffer infoBuffer);

methodmap ClientInfoChangedHook
{
    public native ClientInfoChangedHook(ClientInfoChangedHookCallback callback, HookPriority priority = HookPrio_Default);
    public native void Unhook();
}