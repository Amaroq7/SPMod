/*
 *  Copyright (C) 2018 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  This program is free software: you can redistribute it and/or modify
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

#pragma once

#include "spmod.hpp"

class Timer;

class TimerMngr final : public ITimerMngr
{
public:
    TimerMngr() = default;
    ~TimerMngr() = default;

    template<typename ...Args>
    std::shared_ptr<Timer> createTimerCore(Args... args)
    {
        return m_timers.emplace_back(std::make_shared<Timer>(std::forward<Args>(args)...));
    }

    ITimer *createTimer(float interval,
                        TimerCallback func,
                        void *data,
                        bool pause) override;

    void removeTimer(ITimer *timer) override;
    void execTimer(ITimer *timer) override;
    void execTimers(float time);
    void clearTimers();
    void execTimerCore(std::shared_ptr<Timer> timer);
    void removeTimerCore(std::shared_ptr<Timer> timer);

    /* next execution of timers */
    static inline float m_nextExecution;

private:
    std::vector<std::shared_ptr<Timer>> m_timers;
};

class Timer final : public ITimer
{
    friend void TimerMngr::execTimer(ITimer *timer);
    friend void TimerMngr::execTimerCore(std::shared_ptr<Timer> timer);
    friend void TimerMngr::execTimers(float time);

public:
    Timer() = delete;
    Timer(const Timer &other) = delete;
    Timer(Timer &&other) = default;
    ~Timer() = default;

    Timer(float interval,
          TimerCallback func,
          void *data,
          bool pause);

    float getInterval() const override;
    bool isPaused() const override;
    void setInterval(float newint) override;
    void setPause(bool pause) override;

private:
    bool exec(float time);

    /* interval */
    float m_interval;

    /* callback */
    TimerCallback m_callback;

    /* callback data */
    void *m_data;

    /* Pause state */
    bool m_paused;

    /* last execution */
    float m_lastExec;
};