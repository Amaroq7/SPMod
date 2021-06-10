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
#include <metamodcpp_sdk/engine/IHooks.hpp>

static TypeHandler<SPMod::IHookInfo> gSPGameInitHooks;
static TypeHandler<SPMod::IHookInfo> gSPClientConnectHooks;
static TypeHandler<SPMod::IHookInfo> gSPClientDropHooks;
static TypeHandler<SPMod::IHookInfo> gSPClientPutinServerHooks;
static TypeHandler<Metamod::IHookInfo> gMetaChangeLevelHooks;
static TypeHandler<SPMod::IHookInfo> gSPClientInfoChangedHooks;

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
                continue;
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

        for (auto hook : gSPClientDropHooks)
        {
            if (hook)
            {
                hooks->dropClient()->unregisterHook(hook);
            }
        }

        for (auto hook : gSPClientPutinServerHooks)
        {
            if (hook)
            {
                hooks->clientPutinServer()->unregisterHook(hook);
            }
        }

        Metamod::Engine::IHooks *eHooks = gEngine->getHooks();
        for (auto hook : gMetaChangeLevelHooks)
        {
            if (hook)
            {
                eHooks->changeLevel()->unregisterHook(hook);
            }
        }

        for (auto hook : gSPClientInfoChangedHooks)
        {
            if (hook)
            {
                hooks->clientInfoChanged()->unregisterHook(hook);
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

    if (func)
    {
        return -1;
    }

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

static void GameInitHookCall(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params, bool original)
{
    enum
    {
        arg_hook = 1
    };

    auto initHook = reinterpret_cast<SPMod::IGameInitHook *>(params[arg_hook]);
    (!original) ? initHook->callNext() : initHook->callOriginal();
}

static cell_t GameInitHookChainNext(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    GameInitHookCall(ctx, params, false);
    return 1;
}

static cell_t GameInitHookChainOriginal(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    GameInitHookCall(ctx, params, true);
    return 1;
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

    if (!func)
    {
        return -1;
    }

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

static cell_t ClientConnectHookCall(SourcePawn::IPluginContext *ctx, const cell_t *params, bool original)
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
    return ClientConnectHookCall(ctx, params, false);
}

static cell_t ClientConnectHookChainOriginal(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    return ClientConnectHookCall(ctx, params, true);
}

static cell_t ClientDropHook(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_callback = 1,
        arg_prio
    };

    static SPMod::IHooks *hooks = gSPGlobal->getHooks();

    auto vCbPrio = static_cast<SPMod::HookPriority>(params[arg_prio]);
    SourcePawn::IPluginFunction *func = ctx->GetFunctionById(params[arg_callback]);

    if (!func)
    {
        return -1;
    }

    SPMod::IHookInfo *hookInfo = hooks->dropClient()->registerHook(
        [func](SPMod::IDropClientHook *hook, Metamod::Engine::IEdict *pEntity,
               bool crash, std::string_view string) {

            if (!func->IsRunnable())
            {
                hook->callNext(pEntity, crash, string);
                return;
            }

            func->PushCell(reinterpret_cast<std::intptr_t>(hook));
            func->PushCell(static_cast<cell_t>(pEntity->getIndex()));
            func->PushCell(crash);
            func->PushString(string.data());
            func->Execute(nullptr);
        }, vCbPrio);

    return static_cast<cell_t>(gSPClientDropHooks.create(hookInfo));
}

static cell_t ClientDropHookUnhook(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_hook = 1
    };

    static SPMod::IHooks *hooks = gSPGlobal->getHooks();
    SPMod::IHookInfo *hookInfo = gSPClientDropHooks.get(params[arg_hook]);

    if (!hookInfo)
    {
        return 0;
    }

    hooks->dropClient()->unregisterHook(hookInfo);
    return 1;
}

static void ClientDropHookChainCall(SourcePawn::IPluginContext *ctx, const cell_t *params, bool original)
{
    enum
    {
        arg_hook = 1,
        arg_client,
        arg_crash,
        arg_string
    };

    auto dropHook = reinterpret_cast<SPMod::IDropClientHook *>(params[arg_hook]);

    Metamod::Engine::IEdict *pEdict = gEngine->getEdict(static_cast<std::uint32_t>(params[arg_client]));
    bool crash = params[arg_crash] == 1;

    char *string;
    ctx->LocalToString(params[arg_string], &string);

    (!original) ? dropHook->callNext(pEdict, crash, string)
                : dropHook->callOriginal(pEdict, crash, string);
}

static cell_t ClientDropHookChainNext(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    ClientDropHookChainCall(ctx, params, false);
    return 1;
}

static cell_t ClientDropHookChainOriginal(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    ClientDropHookChainCall(ctx, params, true);
    return 1;
}

static cell_t ClientPutinServerHook(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_callback = 1,
        arg_prio
    };

    static SPMod::IHooks *hooks = gSPGlobal->getHooks();

    auto vCbPrio = static_cast<SPMod::HookPriority>(params[arg_prio]);
    SourcePawn::IPluginFunction *func = ctx->GetFunctionById(params[arg_callback]);

    if (!func)
    {
        return -1;
    }

    SPMod::IHookInfo *hookInfo = hooks->clientPutinServer()->registerHook(
        [func](SPMod::IClientPutinServerHook *hook, Metamod::Engine::IEdict *pEntity) {

            if (!func->IsRunnable())
            {
                hook->callNext(pEntity);
                return;
            }

            func->PushCell(reinterpret_cast<std::intptr_t>(hook));
            func->PushCell(static_cast<cell_t>(pEntity->getIndex()));
            func->Execute(nullptr);
        }, vCbPrio);

    return static_cast<cell_t>(gSPClientPutinServerHooks.create(hookInfo));
}

static cell_t ClientPutinServerHookUnhook(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_hook = 1
    };

    static SPMod::IHooks *hooks = gSPGlobal->getHooks();
    SPMod::IHookInfo *hookInfo = gSPClientPutinServerHooks.get(params[arg_hook]);

    if (!hookInfo)
    {
        return 0;
    }

    hooks->clientPutinServer()->unregisterHook(hookInfo);
    return 1;
}

static void ClientPutinServerHookCall(SourcePawn::IPluginContext *ctx, const cell_t *params, bool original)
{
    enum
    {
        arg_hook = 1,
        arg_client
    };

    auto dropHook = reinterpret_cast<SPMod::IClientPutinServerHook *>(params[arg_hook]);

    Metamod::Engine::IEdict *pEdict = gEngine->getEdict(static_cast<std::uint32_t>(params[arg_client]));

    (!original) ? dropHook->callNext(pEdict)
                : dropHook->callOriginal(pEdict);
}

static cell_t ClientPutinServerHookChainNext(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    ClientPutinServerHookCall(ctx, params, false);
    return 1;
}

static cell_t ClientPutinServerHookChainOriginal(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    ClientPutinServerHookCall(ctx, params, true);
    return 1;
}

static cell_t ChangeLevelHook(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_callback = 1,
        arg_prio
    };

    static Metamod::Engine::IHooks *hooks = gEngine->getHooks();

    auto vCbPrio = static_cast<Metamod::HookPriority>(params[arg_prio]);
    SourcePawn::IPluginFunction *func = ctx->GetFunctionById(params[arg_callback]);

    if (!func)
    {
        return -1;
    }

    Metamod::IHookInfo *hookInfo = hooks->changeLevel()->registerHook(
        [func](Metamod::Engine::IChangeLevelHook *hook, std::string_view map1, std::string_view map2) {

          if (!func->IsRunnable())
          {
              hook->callNext(map1, map2);
              return;
          }

          func->PushCell(reinterpret_cast<std::intptr_t>(hook));
          func->PushString(map1.data());
          func->PushString(map2.data());
          func->Execute(nullptr);
        }, vCbPrio);

    return static_cast<cell_t>(gMetaChangeLevelHooks.create(hookInfo));
}

static cell_t ChangeLevelHookUnhook(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_hook = 1
    };

    static Metamod::Engine::IHooks *hooks = gEngine->getHooks();
    Metamod::IHookInfo *hookInfo = gMetaChangeLevelHooks.get(params[arg_hook]);

    if (!hookInfo)
    {
        return 0;
    }

    hooks->changeLevel()->unregisterHook(hookInfo);
    return 1;
}

static void ChangeLevelHookCall(SourcePawn::IPluginContext *ctx, const cell_t *params, bool original)
{
    enum
    {
        arg_hook = 1,
        arg_map1,
        arg_map2
    };

    auto changeLevelHook = reinterpret_cast<Metamod::Engine::IChangeLevelHook *>(params[arg_hook]);

    char *mapName1;
    ctx->LocalToString(params[arg_map1], &mapName1);

    char *mapName2;
    ctx->LocalToString(params[arg_map2], &mapName2);

    (!original) ? changeLevelHook->callNext(mapName1, mapName2)
                : changeLevelHook->callOriginal(mapName1, mapName2);
}

static cell_t ChangeLevelHookChainNext(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    ChangeLevelHookCall(ctx, params, false);
    return 1;
}

static cell_t ChangeLevelHookChainOriginal(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    ChangeLevelHookCall(ctx, params, true);
    return 1;
}

static cell_t ClientInfoChangedHook(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_callback = 1,
        arg_prio
    };

    static SPMod::IHooks *hooks = gSPGlobal->getHooks();

    auto vCbPrio = static_cast<SPMod::HookPriority>(params[arg_prio]);
    SourcePawn::IPluginFunction *func = ctx->GetFunctionById(params[arg_callback]);

    if (!func)
    {
        return -1;
    }

    SPMod::IHookInfo *hookInfo = hooks->clientInfoChanged()->registerHook(
        [func](SPMod::IClientInfoChangedHook *hook, Metamod::Engine::IEdict *pEntity, Metamod::Engine::InfoBuffer infoBuffer) {

          if (!func->IsRunnable())
          {
              hook->callNext(pEntity, infoBuffer);
              return;
          }

          func->PushCell(reinterpret_cast<std::intptr_t>(hook));
          func->PushCell(static_cast<cell_t>(pEntity->getIndex()));
          func->PushCell(reinterpret_cast<std::intptr_t>(&infoBuffer));
          func->Execute(nullptr);
        }, vCbPrio);

    return static_cast<cell_t>(gSPClientInfoChangedHooks.create(hookInfo));
}

static cell_t ClientInfoChangedHookUnhook(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_hook = 1
    };

    static SPMod::IHooks *hooks = gSPGlobal->getHooks();
    SPMod::IHookInfo *hookInfo = gSPClientInfoChangedHooks.get(params[arg_hook]);

    if (!hookInfo)
    {
        return 0;
    }

    hooks->clientInfoChanged()->unregisterHook(hookInfo);
    return 1;
}

static void ClientInfoChangedHook(SourcePawn::IPluginContext *ctx, const cell_t *params, bool original)
{
    enum
    {
        arg_hook = 1,
        arg_client,
        arg_infoBuffer,
    };

    auto clientInfoChangedHook = reinterpret_cast<SPMod::IClientInfoChangedHook *>(params[arg_hook]);
    auto &infoBuffer = *reinterpret_cast<Metamod::Engine::InfoBuffer *>(params[arg_hook]);
    Metamod::Engine::IEdict *pEdict = gEngine->getEdict(static_cast<std::uint32_t>(params[arg_client]));

    (!original) ? clientInfoChangedHook->callNext(pEdict, infoBuffer)
                : clientInfoChangedHook->callOriginal(pEdict, infoBuffer);
}

static cell_t ClientInfoChangedHookChainNext(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    ClientInfoChangedHook(ctx, params, false);
    return 1;
}

static cell_t ClientInfoChangedHookChainOriginal(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    ClientInfoChangedHook(ctx, params, true);
    return 1;
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

    // Client Drop
    {"ClientDropHook.ClientDropHook", ClientDropHook},
    {"ClientDropHook.Unhook", ClientDropHookUnhook},
    {"ClientDropHookChain.CallNext", ClientDropHookChainNext},
    {"ClientDropHookChain.CallOriginal", ClientDropHookChainOriginal},

    // Client PutinServer
    {"ClientPutinServerHook.ClientPutinServerHook", ClientPutinServerHook},
    {"ClientPutinServerHook.Unhook", ClientPutinServerHookUnhook},
    {"ClientPutinServerHookChain.CallNext", ClientPutinServerHookChainNext},
    {"ClientPutinServerHookChain.CallOriginal", ClientPutinServerHookChainOriginal},

    // Change Level
    {"ChangeLevelHook.ChangeLevelHook", ChangeLevelHook},
    {"ChangeLevelHook.Unhook", ChangeLevelHookUnhook},
    {"ChangeLevelHookChain.CallNext", ChangeLevelHookChainNext},
    {"ChangeLevelHookChain.CallOriginal", ChangeLevelHookChainOriginal},

    // Client Info Changed
    {"ClientInfoChangedHook.ClientInfoChangedHook", ClientInfoChangedHook},
    {"ClientInfoChangedHook.Unhook", ClientInfoChangedHookUnhook},
    {"ClientInfoChangedHookChain.CallNext", ClientInfoChangedHookChainNext},
    {"ClientInfoChangedHookChain.CallOriginal", ClientInfoChangedHookChainOriginal},

    {nullptr, nullptr}
};
