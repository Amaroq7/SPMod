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

#include "spmod.hpp"

class Timer final : public ITimer
{
public:
    Timer() = delete;
    Timer(const Timer &other) = delete;
    Timer(Timer &&other) = default;
    ~Timer() = default;

    Timer(float interval, Callback func, std::any cbData, std::any data, bool pause);

    // ITimer
    float getInterval() const override;
    bool isPaused() const override;
    void setInterval(float newint) override;
    void setPause(bool pause) override;
    std::any getData() const override;
    bool exec() override;

    // Timer
    float getLastExecTime() const;

private:
    /* interval */
    float m_interval;

    /* callback */
    Callback m_callback;

    /* callback data */
    std::any m_cbData;

    /* plugin data */
    std::any m_data;

    /* Pause state */
    bool m_paused;

    /* last execution */
    float m_lastExec;
};

class TimerMngr final : public ITimerMngr
{
public:
    TimerMngr() = default;
    ~TimerMngr() = default;

    Timer *createTimer(float interval,
                       Timer::Callback callback,
                       std::any cbData,
                       std::any data,
                       bool pause = false) override;
    void removeTimer(const ITimer *timer) override;

    void execTimers(float execTime);
    void clearTimers();

    /* next execution of timers */
    static inline float m_nextExecution;

private:
    std::vector<std::unique_ptr<Timer>> m_timers;
};
