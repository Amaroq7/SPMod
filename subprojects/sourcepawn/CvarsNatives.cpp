/*
 *  Copyright (C) 2018-2020 SPMod Development Team
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

#include "CvarsNatives.hpp"
#include "SourcePawnAPI.hpp"
#include "ExtMain.hpp"
#include "StringNatives.hpp"

#include <metamodcpp_sdk/engine/ICvar.hpp>
#include <metamodcpp_sdk/engine/IHooks.hpp>
#include <metamodcpp_sdk/engine/ILibrary.hpp>

TypeHandler<Metamod::Engine::ICvar> gCvarsHandlers;
std::unordered_multimap<Metamod::Engine::ICvar *, Metamod::IHookInfo *> gCvarPluginsCallbacks;
TypeHandler<Metamod::IHookInfo> gCvarsCallbackHandlers;

TypeHandler<Metamod::Engine::ICvarDirectSetHook> gCvarHookChains;

namespace SPExt
{
    bool initCvarsNatives()
    {
        return true;
    }
}

static cell_t CvarRegister(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_name = 1,
        arg_value,
        arg_flags
    };

    char *cvarName, *cvarValue;
    ctx->LocalToString(params[arg_name], &cvarName);
    ctx->LocalToString(params[arg_value], &cvarValue);

    gEngine->registerCvar(cvarName, cvarValue, Metamod::FuncCallType::Direct);

    Metamod::Engine::ICvar *plCvar = gEngine->getCvar(cvarName, Metamod::FuncCallType::Direct);

    if (!plCvar)
        return -1;

    plCvar->setFlags(static_cast<Metamod::Engine::ICvar::Flags>(params[arg_flags]));

    std::size_t id = gCvarsHandlers.getKey(plCvar);

    if (id != static_cast<std::size_t>(-1))
        return id;

    return static_cast<cell_t>(gCvarsHandlers.create(plCvar));
}

static cell_t CvarGetName(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_index = 1,
        arg_buffer,
        arg_size
    };

    cell_t cvarId = params[arg_index];
    if (cvarId < 0)
    {
        ctx->ReportError("Invalid cvar pointer");
        return 0;
    }

    Metamod::Engine::ICvar *cvar = gCvarsHandlers.get(cvarId);
    if (!cvar)
    {
        ctx->ReportError("Cvar not found");
        return 0;
    }

    char *destBuffer;
    ctx->LocalToString(params[arg_buffer], &destBuffer);

    return static_cast<cell_t>(gSPUtils->strCopy(destBuffer, params[arg_size], cvar->getName()));
}

static cell_t CvarGetFloat(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_index = 1
    };

    cell_t cvarId = params[arg_index];
    if (cvarId < 0)
    {
        ctx->ReportError("Invalid cvar pointer");
        return 0;
    }

    Metamod::Engine::ICvar *cvar = gCvarsHandlers.get(cvarId);
    if (!cvar)
    {
        ctx->ReportError("Cvar not found");
        return 0;
    }

    return sp_ftoc(cvar->getValue());
}

static cell_t CvarGetString(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_index = 1,
        arg_buffer,
        arg_size
    };

    cell_t cvarId = params[arg_index];
    if (cvarId < 0)
    {
        ctx->ReportError("Invalid cvar pointer");
        return 0;
    }

    Metamod::Engine::ICvar *cvar = gCvarsHandlers.get(cvarId);
    if (!cvar)
    {
        ctx->ReportError("Cvar not found");
        return 0;
    }

    char *destBuffer;
    ctx->LocalToString(params[arg_buffer], &destBuffer);

    return static_cast<cell_t>(gSPUtils->strCopy(destBuffer, params[arg_size], cvar->getString()));
}

static cell_t CvarGetInt(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_index = 1
    };

    cell_t cvarId = params[arg_index];
    if (cvarId < 0)
    {
        ctx->ReportError("Invalid cvar pointer");
        return 0;
    }

    Metamod::Engine::ICvar *cvar = gCvarsHandlers.get(cvarId);
    if (!cvar)
    {
        ctx->ReportError("Cvar not found");
        return 0;
    }

    return static_cast<cell_t>(cvar->getValue());
}

static cell_t CvarGetFlags(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_index = 1
    };

    cell_t cvarId = params[arg_index];
    if (cvarId < 0)
    {
        ctx->ReportError("Invalid cvar pointer");
        return 0;
    }

    Metamod::Engine::ICvar *cvar = gCvarsHandlers.get(cvarId);
    if (!cvar)
    {
        ctx->ReportError("Cvar not found");
        return 0;
    }

    return static_cast<cell_t>(cvar->getFlags());
}

static cell_t CvarSetFloat(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_index = 1,
        arg_value
    };

    cell_t cvarId = params[arg_index];
    if (cvarId < 0)
    {
        ctx->ReportError("Invalid cvar pointer");
        return 0;
    }

    Metamod::Engine::ICvar *cvar = gCvarsHandlers.get(cvarId);
    if (!cvar)
    {
        ctx->ReportError("Cvar not found");
        return 0;
    }
    cvar->setValue(sp_ctof(params[arg_value]));

    return 1;
}

static cell_t CvarSetString(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_index = 1,
        arg_value
    };

    char *cvarValue;
    ctx->LocalToString(params[arg_value], &cvarValue);

    cell_t cvarId = params[arg_index];
    if (cvarId < 0)
    {
        ctx->ReportError("Invalid cvar pointer");
        return 0;
    }

    Metamod::Engine::ICvar *cvar = gCvarsHandlers.get(cvarId);
    if (!cvar)
    {
        ctx->ReportError("Cvar not found");
        return 0;
    }
    cvar->setString(cvarValue);

    return 1;
}

static cell_t CvarSetInt(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_index = 1,
        arg_value
    };

    cell_t cvarId = params[arg_index];
    if (cvarId < 0)
    {
        ctx->ReportError("Invalid cvar pointer");
        return 0;
    }

    Metamod::Engine::ICvar *cvar = gCvarsHandlers.get(cvarId);
    if (!cvar)
    {
        ctx->ReportError("Cvar not found");
        return 0;
    }
    cvar->setValue(sp_ctof(params[arg_value]));

    return 1;
}

static cell_t CvarAddCallback(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_index = 1,
        cvar_callback
    };

    static Metamod::Engine::ICvarDirectSetHookRegistry *cvarDirectSetRegistry = gEngine->getHooks()->cvarDirectSet();

    cell_t cvarId = params[arg_index];
    if (cvarId < 0)
    {
        ctx->ReportError("Invalid cvar pointer");
        return -1;
    }

    Metamod::Engine::ICvar *plCvar = gCvarsHandlers.get(cvarId);
    if (!plCvar)
    {
        ctx->ReportError("Cvar not found");
        return -1;
    }

    SourcePawn::IPluginFunction *func = ctx->GetFunctionById(params[cvar_callback]);
    if (!func || !func->IsRunnable())
    {
        return -1;
    }

    Metamod::IHookInfo *cvarHook = cvarDirectSetRegistry->registerHook(
        [func, plCvar](Metamod::Engine::ICvarDirectSetHook *hook, Metamod::Engine::ICvar *cvar, std::string_view value) {

            if (!func->IsRunnable() || plCvar != cvar)
            {
                hook->callNext(cvar, value);
                return;
            }

            std::size_t hookId = gCvarHookChains.create(hook);

            func->PushCell(static_cast<cell_t>(hookId));
            func->PushCell(static_cast<cell_t>(gCvarsHandlers.getKey(cvar)));
            func->PushString(value.data());
            func->Execute(nullptr);

            gCvarHookChains.free(hookId); // call in case callNext or callOriginal were not called
        }
    );

    gCvarPluginsCallbacks.emplace(plCvar, cvarHook);

    return static_cast<cell_t>(gCvarsCallbackHandlers.create(cvarHook));
}

static cell_t CvarRemoveCallback(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_index = 1,
        arg_hookInfo = 2,
    };

    static Metamod::Engine::ICvarDirectSetHookRegistry *cvarDirectSetRegistry = gEngine->getHooks()->cvarDirectSet();

    cell_t cvarId = params[arg_index];
    if (cvarId < 0)
    {
        ctx->ReportError("Invalid cvar pointer");
        return 0;
    }

    Metamod::Engine::ICvar *cvar = gCvarsHandlers.get(cvarId);
    if (!cvar)
    {
        ctx->ReportError("Cvar not found");
        return 0;
    }

    Metamod::IHookInfo *cvarHook = gCvarsCallbackHandlers.get(static_cast<std::size_t>(params[arg_hookInfo]));
    if (!cvarHook)
    {
        ctx->ReportError("Invalid hook");
        return 0;
    }

    const auto& [beginIt, endIt] = gCvarPluginsCallbacks.equal_range(cvar);
    for (auto iter = beginIt; iter != endIt; ++iter)
    {
        if (iter->second == cvarHook)
        {
            cvarDirectSetRegistry->unregisterHook(cvarHook);
            return 1;
        }
    }

    return 0;
}

static cell_t CvarSetFlags(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_index = 1,
        arg_flags
    };

    cell_t cvarId = params[arg_index];
    if (cvarId < 0)
    {
        ctx->ReportError("Invalid cvar pointer");
        return 0;
    }

    Metamod::Engine::ICvar *cvar = gCvarsHandlers.get(cvarId);
    if (!cvar)
    {
        ctx->ReportError("Cvar not found");
        return 0;
    }

    cvar->setFlags(static_cast<Metamod::Engine::ICvar::Flags>(params[arg_flags]));
    return 1;
}

static cell_t CvarFind(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_name = 1
    };

    char *cvarName;
    ctx->LocalToString(params[arg_name], &cvarName);

    Metamod::Engine::ICvar *plCvar = gEngine->getCvar(cvarName, Metamod::FuncCallType::Direct);

    if (!plCvar)
        return -1;

    std::size_t id = gCvarsHandlers.getKey(plCvar);

    if (id != static_cast<std::size_t>(-1))
        return static_cast<cell_t>(id);

    return static_cast<cell_t>(gCvarsHandlers.create(plCvar));
}

static cell_t CvarHookChainCall(SourcePawn::IPluginContext *ctx, const cell_t *params, bool original)
{
    enum
    {
        arg_hook = 1,
        arg_cvar,
        arg_value
    };

    std::size_t hookId = static_cast<std::size_t>(params[arg_hook]);
    Metamod::Engine::ICvarDirectSetHook *cvarDirectSetHook = gCvarHookChains.get(hookId);
    if (!cvarDirectSetHook)
    {
        if (original)
        {
            ctx->ReportError("CvarHookChain.callOriginal outside hook or called twice");
        }
        else
        {
            ctx->ReportError("CvarHookChain.callNext outside hook or called twice");
        }
        return 0;
    }

    Metamod::Engine::ICvar *cvar = gCvarsHandlers.get(static_cast<std::size_t>(params[arg_cvar]));

    char *value;
    ctx->LocalToString(params[arg_value], &value);

    (!original) ? cvarDirectSetHook->callNext(cvar, value) :
                  cvarDirectSetHook->callOriginal(cvar, value);

    gCvarHookChains.free(hookId);
    return 1;
}

static cell_t CvarHookChainNext(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    return CvarHookChainCall(ctx, params, false);
}

static cell_t CvarHookChainOriginal(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    return CvarHookChainCall(ctx, params, true);
}

sp_nativeinfo_t gCvarsNatives[] = {{"Cvar.Cvar", CvarRegister},
                                   {"Cvar.GetName", CvarGetName},
                                   {"Cvar.GetFloat", CvarGetFloat},
                                   {"Cvar.GetString", CvarGetString},
                                   {"Cvar.GetInt", CvarGetInt},
                                   {"Cvar.GetFlags", CvarGetFlags},
                                   {"Cvar.SetFloat", CvarSetFloat},
                                   {"Cvar.SetString", CvarSetString},
                                   {"Cvar.SetInt", CvarSetInt},
                                   {"Cvar.SetFlags", CvarSetFlags},

                                   {"Cvar.FloatValue.get", CvarGetFloat},
                                   {"Cvar.FloatValue.set", CvarSetFloat},
                                   {"Cvar.IntValue.get", CvarGetInt},
                                   {"Cvar.IntValue.set", CvarSetInt},
                                   {"Cvar.Flags.get", CvarGetFlags},
                                   {"Cvar.Flags.set", CvarSetFlags},

                                   {"Cvar.AddHookOnChange", CvarAddCallback},
                                   {"Cvar.RemoveHookOnChange", CvarRemoveCallback},

                                   {"CvarHookChain.callNext", CvarHookChainNext},
                                   {"CvarHookChain.callOriginal", CvarHookChainOriginal},

                                   {"FindCvar", CvarFind},
                                   {nullptr, nullptr}};
