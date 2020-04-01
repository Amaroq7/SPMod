/*
 *  Copyright (C) 2020 SPMod Development Team
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

std::unordered_multimap<SPMod::IVTableHook *, SourcePawn::IPluginFunction *> gVTableHook;
std::unordered_multimap<SPMod::IVTableHook *, SourcePawn::IPluginFunction *> gVTableHookPost;
TypeHandler<SPMod::IVTableHook> gVTableHandlers;

static cell_t VTableHookRegister(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_vfunc = 1,
        arg_classname,
        arg_callback,
        arg_post
    };

    auto vFunc = static_cast<SPMod::IVTableHookManager::vFuncType>(params[arg_vfunc]);

    char *className;
    ctx->LocalToString(params[arg_classname], &className);
    SPMod::IVTableHookManager *vTableMngr = gSPGlobal->getVTableManager();
    SPMod::IVTableHook *hook = vTableMngr->registerHook(vFunc, className);
    if (!hook)
        return -1;

    if (!params[arg_post])
    {
        gVTableHook.emplace(hook, ctx->GetFunctionById(params[arg_callback]));
    }
    else
    {
        gVTableHookPost.emplace(hook, ctx->GetFunctionById(params[arg_callback]));
    }

    return gVTableHandlers.create(hook);
}

static cell_t VTableHookSetFloat(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_hook = 1,
        arg_pos,
        arg_paramf
    };

    if (!params[arg_pos])
    {
        return 0;
    }

    SPMod::IVTableHook *hook = gVTableHandlers.get(params[arg_hook]);
    if (!hook)
    {
        return 0;
    }

    auto &hookParams = hook->getParams();
    if (!hookParams.size() || static_cast<std::size_t>(params[arg_pos]) >= hookParams.size() ||
        !std::holds_alternative<float>(hookParams[params[arg_pos]]))
    {
        return 0;
    }

    hookParams[params[arg_pos]] = sp_ctof(params[arg_paramf]);

    return 1;
}

sp_nativeinfo_t gVTableNatives[] = {{"VTHook.VTHook", VTableHookRegister},
                                    {"VTHook.SetFloat", VTableHookSetFloat},
                                    {nullptr, nullptr}};