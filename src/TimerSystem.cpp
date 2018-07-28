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

#include "spmod.hpp"

Timer::Timer(std::size_t id,
             float interval,
             std::variant<SourcePawn::IPluginFunction *, TimerCallback> &&func,
             std::variant<cell_t, void *> &&data,
             bool pause) : m_id(id),
                           m_interval(interval),
                           m_callback(func),
                           m_data(data),
                           m_paused(pause),
                           m_lastExec(gpGlobals->time)
{
    if (m_interval <= 0.0f)
        throw std::runtime_error("Interval lesser or equal to 0");
}

float Timer::getInterval() const
{
    return m_interval;
}
std::size_t Timer::getId() const
{
    return m_id;
}
bool Timer::isPaused() const
{
    return m_paused;
}
void Timer::setInterval(float newint)
{
    m_interval = newint;
    m_lastExec = gpGlobals->time;
}

void Timer::setPause(bool pause)
{
    m_paused = pause;

    // Delay exec of timer by its interval
    if (!pause)
        m_lastExec = gpGlobals->time;
}

bool Timer::exec(float gltime)
{
    m_lastExec = gltime;

    // First check for plugin timer
    try
    {
        cell_t result;
        auto *func = std::get<SourcePawn::IPluginFunction *>(m_callback);
        func->PushCell(m_id);
        func->PushCell(std::get<cell_t>(m_data));
        func->Execute(&result);

        return result == IForward::ReturnValue::PluginIgnored;
    }
    catch (const std::bad_variant_access &e [[maybe_unused]])
    {
        auto func = std::get<TimerCallback>(m_callback);
        return func(this, std::get<void *>(m_data));
    }
}

ITimer *TimerMngr::createTimer(float interval,
                               TimerCallback func,
                               void *data,
                               bool pause)
{
    try
    {
        return createTimerCore(interval, func, data, pause).get();
    }
    catch (const std::runtime_error &e [[maybe_unused]])
    {
        return nullptr;
    }
}

void TimerMngr::removeTimer(ITimer *timer)
{
    auto iter = m_timers.begin();
    while (iter != m_timers.end())
    {
        if (iter->get() == timer)
        {
            m_timers.erase(iter);
            break;
        }
        ++iter;
    }
}

void TimerMngr::removeTimerCore(std::size_t id)
{
    auto iter = m_timers.begin();
    while (iter != m_timers.end())
    {
        if ((*iter)->getId() == id)
        {
            m_timers.erase(iter);
            break;
        }
        ++iter;
    }
}

std::shared_ptr<Timer> TimerMngr::getTimer(std::size_t id) const
{
    auto iter = m_timers.begin();
    while (iter != m_timers.end())
    {
        if ((*iter)->getId() == id)
        {
            return *iter;
        }
        ++iter;
    }

    return nullptr;
}

void TimerMngr::execTimers(float gltime)
{
    auto iter = m_timers.begin();
    while (iter != m_timers.end())
    {
        std::shared_ptr<Timer> task = *iter;

        if (task->isPaused() || task->m_lastExec + task->m_interval > gltime)
        {
            ++iter;
            continue;
        }

        if (!task->exec(gltime))
            iter = m_timers.erase(iter);
        else
            ++iter;
    }
}

void TimerMngr::execTimerCore(std::shared_ptr<Timer> timer)
{
    auto iter = m_timers.begin();
    float curTime = gpGlobals->time;
    while (iter != m_timers.end())
    {
        std::shared_ptr<Timer> task = *iter;

        if (task != timer)
        {
            ++iter;
            continue;
        }

        if (!task->exec(curTime))
            m_timers.erase(iter);

        break;
    }
}

void TimerMngr::execTimer(ITimer *timer)
{
    auto iter = m_timers.begin();
    float curTime = gpGlobals->time;
    while (iter != m_timers.end())
    {
        std::shared_ptr<Timer> task = *iter;

        if (task.get() != timer)
        {
            ++iter;
            continue;
        }

        if (!task->exec(curTime))
            m_timers.erase(iter);
        
        break;
    }
}

void TimerMngr::clearTimers()
{
    m_timers.clear();
    m_id = 0;
}