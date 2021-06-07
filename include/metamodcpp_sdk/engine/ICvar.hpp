/*
 *  Copyright (C) 2020 Metamod++ Development Team
 *
 *  This file is part of Metamod++.
 *
 *  Metamod++ is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  Metamod++ is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with Metamod++.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <cinttypes>
#include <string_view>

#if defined META_CORE
typedef struct cvar_s cvar_t;
#endif

namespace Metamod::Engine
{
    class ICvar
    {
    public:
        enum class Flags : std::uint16_t
        {
            // no extra flags
            None = 0,

            // set to cause it to be saved to vars.rc
            Archive = (1 << 0),

            // changes the client's info string
            UserInfo = (1 << 1),

            // notifies players when changed
            Server = (1 << 2),

            // defined by external DLL
            ExtDll = (1 << 3),

            // defined by the client dll
            ClientDll = (1 << 4),

            // It's a server cvar, but we don't send the data since it's a password, etc.  Sends 1 if it's not
            // bland/zero, 0 otherwise as value
            Protected = (1 << 5),

            // This cvar cannot be changed by clients connected to a multiplayer server.
            SPOnly = (1 << 6),

            // This cvar's string cannot contain unprintable characters ( e.g., used for player name etc ).
            PrintableOnly = (1 << 7),

            // If this is a FCVAR_SERVER, don't log changes to the log file / console if we are creating a log
            Unlogged = (1 << 8),

            // strip trailing/leading white space from this cvar
            NoExtraWhiteSpace = (1 << 9)
        };

    public:
        virtual ~ICvar() = default;

        [[nodiscard]] virtual std::string_view getName() const = 0;
        [[nodiscard]] virtual std::string_view getString() const = 0;
        [[nodiscard]] virtual Flags getFlags() const = 0;
        [[nodiscard]] virtual float getValue() const = 0;

        virtual void setString(std::string_view string) = 0;
        virtual void setFlags(Flags flags) = 0;
        virtual void setValue(float value) = 0;

#if defined META_CORE
        virtual explicit operator cvar_t *() const = 0;
#endif
    };
}