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

#include "ExtMain.hpp"

TypeHandler<SPMod::ITimer> gTimerHandlers;
static cell_t TimerCtor(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_interval = 1,
        arg_func,
        arg_data,
        arg_exec,
        arg_pause
    };

    SPMod::ITimerMngr *timerMngr = gSPGlobal->getTimerManager();
    SourcePawn::IPluginFunction *func = ctx->GetFunctionById(params[arg_func]);
    SPMod::ITimer *timer;

    try
    {
        timer = timerMngr->createTimer(sp_ctof(params[arg_interval]), SPExt::Listener::TimerCallback, func,
                                       const_cast<cell_t *>(&params[arg_data]), params[arg_pause]);
    }
    catch (const std::runtime_error &e)
    {
        ctx->ReportError(e.what());
        return -1;
    }

    if (params[arg_exec])
        timerMngr->execTimer(timer);

    return gTimerHandlers.create(timer);
}

static cell_t PauseGet(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_id = 1
    };

    SPMod::ITimer *timer = gTimerHandlers.get(params[arg_id]);
    if (!timer)
    {
        ctx->ReportError("Invalid timer id (%i)", params[arg_id]);
        return 0;
    }

    return timer->isPaused();
}

static cell_t IntervalGet(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_id = 1
    };

    SPMod::ITimer *timer = gTimerHandlers.get(params[arg_id]);
    if (!timer)
    {
        ctx->ReportError("Invalid timer id (%i)", params[arg_id]);
        return 0;
    }

    return sp_ftoc(timer->getInterval());
}

static cell_t PauseSet(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_id = 1,
        arg_pause
    };

    SPMod::ITimer *timer = gTimerHandlers.get(params[arg_id]);
    if (!timer)
    {
        ctx->ReportError("Invalid timer id (%i)", params[arg_id]);
        return 0;
    }

    timer->setPause(params[arg_pause]);
    return 1;
}

static cell_t IntervalSet(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_id = 1,
        arg_interval
    };

    SPMod::ITimer *timer = gTimerHandlers.get(params[arg_id]);
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

static cell_t Trigger(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_id = 1
    };

    SPMod::ITimerMngr *timerMngr = gSPGlobal->getTimerManager();
    SPMod::ITimer *timer = gTimerHandlers.get(params[arg_id]);
    if (!timer)
    {
        ctx->ReportError("Invalid timer id (%i)", params[arg_id]);
        return 0;
    }

    timerMngr->execTimer(timer);
    return 1;
}

static cell_t Remove(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_id = 1
    };

    SPMod::ITimerMngr *timerMngr = gSPGlobal->getTimerManager();
    timerMngr->removeTimer(gTimerHandlers.get(params[arg_id]));
    return 1;
}

sp_nativeinfo_t gTimerNatives[] = {{"Timer.Timer", TimerCtor},
                                   {"Timer.Paused.get", PauseGet},
                                   {"Timer.Interval.get", IntervalGet},
                                   {"Timer.Paused.set", PauseSet},
                                   {"Timer.Interval.set", IntervalSet},
                                   {"Timer.Trigger", Trigger},
                                   {"Timer.Remove", Remove},
                                   {nullptr, nullptr}};