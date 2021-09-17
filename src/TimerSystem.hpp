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

#pragma once

#include <ITimerSystem.hpp>

using namespace SPMod;

class Timer final : public ITimer
{
public:
    Timer() = delete;
    Timer(const Timer &other) = delete;
    Timer(Timer &&other) = default;
    ~Timer() final = default;

    Timer(float interval, Callback func, bool pause);

    // ITimer
    [[nodiscard]] float getInterval() const final;
    [[nodiscard]] bool isPaused() const final;
    void setInterval(float interval) final;
    void setPause(bool pause) final;

    // Timer
    [[nodiscard]] float getLastExecTime() const;
    [[nodiscard]] Callback getCallback() const;
    void setLastExecTime(float lastExec);

private:
    /* interval */
    float m_interval;

    /* callback */
    Callback m_callback;

    /* Pause state */
    bool m_paused;

    /* last execution */
    float m_lastExec;
};

class TimerMngr final : public ITimerMngr
{
public:
    TimerMngr() = default;
    ~TimerMngr() final = default;

    nstd::observer_ptr<ITimer> createTimer(float interval, Timer::Callback callback, bool pause) final;
    void removeTimer(nstd::observer_ptr<ITimer> timer) final;
    void execTimer(nstd::observer_ptr<ITimer> timer) final;

    void execTimers(float execTime);
    void clearTimers();

    /* next execution of timers */
    static inline float m_nextExecution;

private:
    static bool execTimer(const std::unique_ptr<Timer>& timer);

private:
    std::vector<std::unique_ptr<Timer>> m_timers;
};
