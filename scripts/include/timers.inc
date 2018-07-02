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

#if defined _timers_included
    #endinput
#endif
#define _timers_included

typedef TimerCallback = function bool (Timer timer, any data) 

methmodmap Timer
{
    public native Timer(float interval, TimerCallback func, any data = 0, bool exec = false, bool pause = false);
    public native void Remove();
    public native void Trigger();
    property float Interval
    {
        public native get();
        public native set(float interval);
    }
    property bool Paused
    {
        public native get();
        public native set(bool pause);
    }
};