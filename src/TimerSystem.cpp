/*
 *  Copyright (C) 2018-2021 SPMod Development Team
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

#include "TimerSystem.hpp"
#include "AnubisInit.hpp"

Timer::Timer(float interval, Timer::Callback func, bool pause)
    : m_interval(interval), m_callback(std::move(func)), m_paused(pause),
      m_lastExec(gEngine->getTime())
{
    if (m_interval <= 0.0f)
        throw std::runtime_error("Interval lesser than or equal to 0");

    if (!func)
        throw std::runtime_error("Invalid function passed");
}

float Timer::getInterval() const
{
    return m_interval;
}

bool Timer::isPaused() const
{
    return m_paused;
}
void Timer::setInterval(float interval)
{
    m_interval = interval;
    m_lastExec = gEngine->getTime();
}

void Timer::setPause(bool pause)
{
    m_paused = pause;

    // Delay exec of timer by its interval
    if (!pause)
        m_lastExec = gEngine->getTime();
}

float Timer::getLastExecTime() const
{
    return m_lastExec;
}

ITimer::Callback Timer::getCallback() const
{
    return m_callback;
}

void Timer::setLastExecTime(float lastExec)
{
    m_lastExec = lastExec;
}

nstd::observer_ptr<ITimer> TimerMngr::createTimer(float interval, Timer::Callback func, bool pause)
{
    try
    {
        return m_timers.emplace_back(std::make_unique<Timer>(interval, func, pause));
    }
    catch (const std::runtime_error &e [[maybe_unused]])
    {
        return {};
    }
}

void TimerMngr::execTimers(float execTime)
{
    auto iter = m_timers.begin();
    while (iter != m_timers.end())
    {
        const auto &task = *iter;

        if (task->isPaused() || task->getLastExecTime() + task->getInterval() > execTime)
        {
            ++iter;
            continue;
        }

        if (!execTimer(task))
        {
            iter = m_timers.erase(iter);
        }
        else
            ++iter;
    }
}

void TimerMngr::clearTimers()
{
    m_timers.clear();
}

void TimerMngr::removeTimer(nstd::observer_ptr<ITimer> timer)
{
    auto iter = m_timers.begin();
    while (iter != m_timers.end())
    {
        if (*iter == timer)
        {
            m_timers.erase(iter);
            break;
        }
        ++iter;
    }
}

void TimerMngr::execTimer(nstd::observer_ptr<ITimer> timer)
{
    if (timer)
    {
        return;
    }

    for (const auto &timerInternal : m_timers)
    {
        if (timerInternal == timer)
        {
            execTimer(timerInternal);
            break;
        }
    }
}

bool TimerMngr::execTimer(const std::unique_ptr<Timer> &timer)
{
    ITimer::Return result = timer->getCallback()(nstd::observer_ptr<ITimer>(timer));
    timer->setLastExecTime(gEngine->getTime());

    return result == ITimer::Return::Continue;
}
