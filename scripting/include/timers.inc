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

#if defined _timers_included
    #endinput
#endif
#define _timers_included

enum Timer
{
    INVALID_TIMER = -1
}

enum TimerReturn
{
    Timer_Continue = 0,
    Timer_Stop
}

typeset TimerCallback
{
    function TimerReturn (Timer timer);
    function TimerReturn (Timer timer, any data);
};

methodmap Timer
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