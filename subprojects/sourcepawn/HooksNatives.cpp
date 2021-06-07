/*
 *  Copyright (C) 2021 SPMod Development Team
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

#include "HooksNatives.hpp"
#include "ExtMain.hpp"
#include "AdapterInterface.hpp"
#include "PluginSystem.hpp"

#include "StringNatives.hpp"

#include <metamodcpp_sdk/engine/ILibrary.hpp>

static TypeHandler<SPMod::IHookInfo> gSPGameInitHooks;
static TypeHandler<SPMod::IHookInfo> gSPClientConnectHooks;

static std::string gRejectReason;
constexpr const std::size_t REASON_REJECT_MAX_LEN = 128;

namespace SPExt
{
    void uninstallHooks()
    {
        SPMod::IHooks *hooks = gSPGlobal->getHooks();
        PluginMngr *pluginMngr = gAdapterInterface->getPluginMngrImpl();

        for (const auto &[identity, plugin] : pluginMngr->getPluginsListImpl())
        {
            SourcePawn::IPluginFunction *func = plugin->getRuntime()->GetFunctionByName("UninstallHooks");

            if (!func || !func->IsRunnable())
            {
                return;
            }

            func->Execute(nullptr);
        }

        for (auto hook : gSPGameInitHooks)
        {
            if (hook)
            {
                hooks->gameInit()->unregisterHook(hook);
            }
        }

        for (auto hook : gSPClientConnectHooks)
        {
            if (hook)
            {
                hooks->clientConnect()->unregisterHook(hook);
            }
        }
    }
}

static cell_t GameInitHook(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_callback = 1,
        arg_prio
    };

    static SPMod::IHooks *hooks = gSPGlobal->getHooks();

    auto vCbPrio = static_cast<SPMod::HookPriority>(params[arg_prio]);
    SourcePawn::IPluginFunction *func = ctx->GetFunctionById(params[arg_callback]);

    SPMod::IHookInfo *hookInfo = hooks->gameInit()->registerHook(
        [func](SPMod::IGameInitHook *hook) {

          if (!func->IsRunnable())
          {
              hook->callNext();
          }

          func->PushCell(reinterpret_cast<std::intptr_t>(hook));
          func->Execute(nullptr);
        }, vCbPrio);

    return static_cast<cell_t>(gSPGameInitHooks.create(hookInfo));
}

static cell_t GameInitHookUnhook(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_hook = 1
    };

    static SPMod::IHooks *hooks = gSPGlobal->getHooks();
    SPMod::IHookInfo *hookInfo = gSPGameInitHooks.get(params[arg_hook]);

    if (!hookInfo)
    {
        return 0;
    }

    hooks->gameInit()->unregisterHook(hookInfo);
    return 1;
}

static cell_t GameInitHookChainCall(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params, bool original)
{
    enum
    {
        arg_hook = 1
    };

    auto initHook = reinterpret_cast<SPMod::IGameInitHook *>(params[arg_hook]);
    (!original) ? initHook->callNext() : initHook->callOriginal();

    return 1;
}

static cell_t GameInitHookChainNext(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    return GameInitHookChainCall(ctx, params, false);
}

static cell_t GameInitHookChainOriginal(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    return GameInitHookChainCall(ctx, params, true);
}

static cell_t ClientConnectHook(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_callback = 1,
        arg_prio
    };

    static SPMod::IHooks *hooks = gSPGlobal->getHooks();

    auto vCbPrio = static_cast<SPMod::HookPriority>(params[arg_prio]);
    SourcePawn::IPluginFunction *func = ctx->GetFunctionById(params[arg_callback]);

    SPMod::IHookInfo *hookInfo = hooks->clientConnect()->registerHook(
        [func](SPMod::IClientConnectHook *hook, Metamod::Engine::IEdict *pEntity,
               std::string_view name, std::string_view address, std::string &rejectReason) {

          if (!func->IsRunnable())
          {
              return hook->callNext(pEntity, name, address, rejectReason);
          }

          cell_t result = 0;
          func->PushCell(reinterpret_cast<std::intptr_t>(hook));
          func->PushCell(static_cast<cell_t>(pEntity->getIndex()));
          func->PushString(name.data());
          func->PushString(address.data());
          func->PushStringEx(rejectReason.data(), REASON_REJECT_MAX_LEN, SM_PARAM_STRING_UTF8 | SM_PARAM_STRING_COPY, SM_PARAM_COPYBACK);
          func->Execute(&result);

          return result == 1;
        }, vCbPrio);

    return static_cast<cell_t>(gSPClientConnectHooks.create(hookInfo));
}

static cell_t ClientConnectUnhook(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_hook = 1
    };

    static SPMod::IHooks *hooks = gSPGlobal->getHooks();
    SPMod::IHookInfo *hookInfo = gSPClientConnectHooks.get(params[arg_hook]);

    if (!hookInfo)
    {
        return 0;
    }

    hooks->clientConnect()->unregisterHook(hookInfo);
    return 1;
}

static cell_t ClientConnectHookChainCall(SourcePawn::IPluginContext *ctx, const cell_t *params, bool original)
{
    enum
    {
        arg_hook = 1,
        arg_client,
        arg_name,
        arg_address,
        arg_reason
    };

    static std::uint32_t numOfCalls = 0;

    auto connectHook = reinterpret_cast<SPMod::IClientConnectHook *>(params[arg_hook]);

    Metamod::Engine::IEdict *pEdict = gEngine->getEdict(static_cast<std::uint32_t>(params[arg_client]));

    char *clientName;
    ctx->LocalToString(params[arg_name], &clientName);

    char *ipAddress;
    ctx->LocalToString(params[arg_address], &ipAddress);

    char *rejectReason;
    ctx->LocalToString(params[arg_reason], &rejectReason);

    ++numOfCalls;

    gRejectReason = rejectReason;
    gRejectReason.resize(REASON_REJECT_MAX_LEN);

    bool result = (!original) ? connectHook->callNext(pEdict, clientName, ipAddress, gRejectReason)
                : connectHook->callOriginal(pEdict, clientName, ipAddress, gRejectReason);

    if (--numOfCalls < 1)
    {
        gSPUtils->strCopy(rejectReason, REASON_REJECT_MAX_LEN, gRejectReason);
    }

    return static_cast<cell_t>(result);
}

static cell_t ClientConnectHookChainNext(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    return ClientConnectHookChainCall(ctx, params, false);
}

static cell_t ClientConnectHookChainOriginal(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    return ClientConnectHookChainCall(ctx, params, true);
}

sp_nativeinfo_t gHooksNatives[] = {
    // GameInit
    {"GameInitHook.GameInitHook", GameInitHook},
    {"GameInitHook.Unhook", GameInitHookUnhook},
    {"GameInitHookChain.CallNext", GameInitHookChainNext},
    {"GameInitHookChain.CallOriginal", GameInitHookChainOriginal},

    // Client Connect
    {"ClientConnectHook.ClientConnectHook", ClientConnectHook},
    {"ClientConnectHook.Unhook", ClientConnectUnhook},
    {"ClientConnectHookChain.CallNext", ClientConnectHookChainNext},
    {"ClientConnectHookChain.CallOriginal", ClientConnectHookChainOriginal},


    {nullptr, nullptr}
};
