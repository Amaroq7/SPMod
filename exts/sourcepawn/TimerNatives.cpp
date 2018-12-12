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

static cell_t TimerCtor(SourcePawn::IPluginContext *ctx,
                        const cell_t *params)
{
    enum { arg_interval = 1, arg_func, arg_data, arg_exec, arg_pause };
    const std::unique_ptr<TimerMngr> &timerMngr = gSPGlobal->getTimerManagerCore();
    SourcePawn::IPluginFunction *func = ctx->GetFunctionById(params[arg_func]);
    std::shared_ptr<Timer> timer;

    try
    {
        timer = timerMngr->createTimerCore(sp_ctof(params[arg_interval]), func, params[arg_data], params[arg_pause]);
    }
    catch (const std::runtime_error &e)
    {
        ctx->ReportError(e.what());
        return -1;
    }

    if (params[arg_exec])
        timerMngr->execTimerCore(timer);

    return timer->getId();
}

static cell_t PauseGet(SourcePawn::IPluginContext *ctx,
                       const cell_t *params)
{
    enum { arg_id = 1 };
    const std::unique_ptr<TimerMngr> &timerMngr = gSPGlobal->getTimerManagerCore();
    std::shared_ptr<Timer> timer = timerMngr->getTimer(params[arg_id]);

    if (!timer)
    {
        ctx->ReportError("Invalid timer id (%i)", params[arg_id]);
        return 0;
    }

    return timer->isPaused();
}

static cell_t IntervalGet(SourcePawn::IPluginContext *ctx,
                          const cell_t *params)
{
    enum { arg_id = 1 };
    const std::unique_ptr<TimerMngr> &timerMngr = gSPGlobal->getTimerManagerCore();
    std::shared_ptr<Timer> timer = timerMngr->getTimer(params[arg_id]);

    if (!timer)
    {
        ctx->ReportError("Invalid timer id (%i)", params[arg_id]);
        return 0;
    }

    return sp_ftoc(timer->getInterval());
}

static cell_t PauseSet(SourcePawn::IPluginContext *ctx,
                       const cell_t *params)
{
    enum { arg_id = 1, arg_pause };
    const std::unique_ptr<TimerMngr> &timerMngr = gSPGlobal->getTimerManagerCore();
    std::shared_ptr<Timer> timer = timerMngr->getTimer(params[arg_id]);

    if (!timer)
    {
        ctx->ReportError("Invalid timer id (%i)", params[arg_id]);
        return 0;
    }

    timer->setPause(params[arg_pause]);
    return 1;
}

static cell_t IntervalSet(SourcePawn::IPluginContext *ctx,
                          const cell_t *params)
{
    enum { arg_id = 1, arg_interval };
    const std::unique_ptr<TimerMngr> &timerMngr = gSPGlobal->getTimerManagerCore();
    std::shared_ptr<Timer> timer = timerMngr->getTimer(params[arg_id]);

    if (!timer)
    {
        ctx->ReportError("Invalid timer id (%i)", params[arg_id]);
        return 0;
    }

    float newInterval = sp_ctof(params[arg_interval]);
    if (newInterval <= 0.0f)
    {
        ctx->ReportError("Interval lesser or equal to 0");
        return 0;
    }

    timer->setInterval(newInterval);
    return 1;
}

static cell_t Trigger(SourcePawn::IPluginContext *ctx,
                      const cell_t *params)
{
    enum { arg_id = 1 };
    const std::unique_ptr<TimerMngr> &timerMngr = gSPGlobal->getTimerManagerCore();
    std::shared_ptr<Timer> timer = timerMngr->getTimer(params[arg_id]);

    if (!timer)
    {
        ctx->ReportError("Invalid timer id (%i)", params[arg_id]);
        return 0;
    }

    timerMngr->execTimerCore(timer);
    return 1;
}

static cell_t Remove(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                     const cell_t *params)
{
    enum { arg_id = 1 };
    const std::unique_ptr<TimerMngr> &timerMngr = gSPGlobal->getTimerManagerCore();

    timerMngr->removeTimerCore(params[arg_id]);
    return 1;
}

sp_nativeinfo_t gTimerNatives[] =
{
    { "Timer.Timer",        TimerCtor   },
    { "Timer.Paused.get",   PauseGet    },
    { "Timer.Interval.get", IntervalGet },
    { "Timer.Paused.set",   PauseSet    },
    { "Timer.Interval.set", IntervalSet },
    { "Timer.Trigger",      Trigger     },
    { "Timer.Remove",       Remove      },
    { nullptr,              nullptr     }
};