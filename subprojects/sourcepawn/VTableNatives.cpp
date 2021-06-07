/*
 *  Copyright (C) 2020-2021 SPMod Development Team
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

#include "VTableNatives.hpp"
#include "SourcePawnAPI.hpp"
#include "ExtMain.hpp"

#include "PlayerNatives.hpp"

#include <metamodcpp_sdk/game/IBasePlayer.hpp>
#include <metamodcpp_sdk/engine/IEdict.hpp>

TypeHandler<Metamod::IHookInfo> gVTableHandlers;
TypeHandler<Metamod::Game::IBasePlayerSpawnHook> gBasePlayerSpawnHooks;

static cell_t VTableBasePlayerSpawnHookRegister(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    static Metamod::Game::IBasePlayerHooks *plrHooks = gGame->getCBasePlayerHooks();

    enum
    {
        arg_callback = 1,
        arg_prio
    };

    auto vCbPrio = static_cast<Metamod::HookPriority>(params[arg_prio]);
    SourcePawn::IPluginFunction *func = ctx->GetFunctionById(params[arg_callback]);
    Metamod::IHookInfo *hookInfo;

    hookInfo = plrHooks->spawn()->registerHook(
        [func](Metamod::Game::IBasePlayerSpawnHook *hook, Metamod::Game::IBasePlayer *plr) {

            if (!func->IsRunnable())
            {
                hook->callNext(plr);
                return;
            }

            std::size_t hookId = gBasePlayerSpawnHooks.create(hook);

            func->PushCell(static_cast<cell_t>(hookId));
            func->PushCell(static_cast<cell_t>(plr->edict()->getIndex()));
            func->Execute(nullptr);

            gBasePlayerSpawnHooks.free(hookId);
        },
        vCbPrio);

    return static_cast<cell_t>(gVTableHandlers.create(hookInfo));
}

static cell_t VTableBasePlayerSpawnHookCall(SourcePawn::IPluginContext *ctx, const cell_t *params, bool original)
{
    enum
    {
        arg_hook = 1,
        arg_player
    };

    auto hookId = static_cast<std::size_t>(params[arg_hook]);
    Metamod::Game::IBasePlayerSpawnHook *spawnHook = gBasePlayerSpawnHooks.get(hookId);
    if (!spawnHook)
    {
        if (original)
        {
            ctx->ReportError("Cannot call BasePlayerSpawnHookChain.callOriginal outside hook or called twice");
        }
        else
        {
            ctx->ReportError("Cannot call BasePlayerSpawnHookChain.callNext outside hook or called twice");
        }
        return 0;
    }

    SPMod::IPlayer *plr = gSPPlrMngr->getPlayer(static_cast<std::uint32_t>(params[arg_player]));

    (!original) ? spawnHook->callNext(plr->basePlayer()) :
                  spawnHook->callOriginal(plr->basePlayer());

    gBasePlayerSpawnHooks.free(hookId);
    return 1;
}

static cell_t VTableBasePlayerSpawnHookNext(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    return VTableBasePlayerSpawnHookCall(ctx, params, false);
}

static cell_t VTableBasePlayerSpawnHookOriginal(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    return VTableBasePlayerSpawnHookCall(ctx, params, true);
}

static cell_t VTableBasePlayerSpawnHookUnregister(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_hook = 1
    };

    if (!gBasePlayerSpawnHooks.empty())
    {
        ctx->ReportError("Cannot unregister hook inside in the callback function");
        return 0;
    }

    static Metamod::Game::IBasePlayerHooks *plrHooks = gGame->getCBasePlayerHooks();
    Metamod::IHookInfo *hookInfo = gVTableHandlers.get(params[arg_hook]);

    if (!hookInfo)
    {
        return 0;
    }

    plrHooks->spawn()->unregisterHook(hookInfo);
    return 1;
}

sp_nativeinfo_t gVTableNatives[] = {{"VTBasePlayerHookSpawn", VTableBasePlayerSpawnHookRegister},
                                    {"BasePlayerSpawnHookChain.CallNext", VTableBasePlayerSpawnHookNext},
                                    {"BasePlayerSpawnHookChain.CallOriginal", VTableBasePlayerSpawnHookOriginal},
                                    {"VTBasePlayerUnHookSpawn", VTableBasePlayerSpawnHookUnregister},
                                    {nullptr, nullptr}};