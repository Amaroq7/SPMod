/*
 *  Copyright (C) 2018-2019 SPMod Development Team
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

#if defined _clients_included
    #endinput
#endif
#define _clients_included

methodmap Client
{
    public native int GetName(char[] buffer, int size);
    public native int GetIP(char[] buffer, int size, bool port = false);
    public native int GetSteamID(char[] buffer, int size);

    property int Index {
        public native get();
    }
    property int UserID {
        public native get();
    }
    property bool Alive {
        public native get();
    }
    property bool Connected {
        public native get();
    }
    property bool Fake {
        public native get();
    }
    property bool HLTV {
        public native get();
    }
    property bool InGame {
        public native get();
    }
}
