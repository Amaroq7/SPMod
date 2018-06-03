/*  SPMod - SourcePawn Scripting Engine for Half-Life
 *  Copyright (C) 2018  SPMod Development Team
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

// native Forward(const char[] name, ExecType exectype, int plid, ParamType ...)
static cell_t forwardCtor(SourcePawn::IPluginContext *ctx,
                          const cell_t *params)
{
    char *fwdName;
    auto execType = static_cast<IForward::ExecType>(params[2]);
    cell_t pluginId = params[3];
    const std::unique_ptr<ForwardMngr> &fwdMngr = gSPGlobal->getForwardManagerCore();

    ctx->LocalToString(params[1], &fwdName);

    size_t fwdParamsNum = params[0] - 3;
    if (fwdParamsNum > SP_MAX_EXEC_PARAMS)
        return -1;

    std::array<IForward::ParamType, SP_MAX_EXEC_PARAMS> fwdParamsList;
    for (size_t i = 0; i < fwdParamsNum; ++i)
    {
        cell_t *paramType;
        ctx->LocalToPhysAddr(params[i + 4], &paramType);
        fwdParamsList.at(i) = static_cast<IForward::ParamType>(*paramType);
    }

    // If single forward search for shared_ptr of that plugin
    std::shared_ptr<Plugin> plugin;
    if (pluginId > -1)
    {
        plugin = gSPGlobal->getPluginManagerCore()->getPluginCore(pluginId);
        if (!plugin)
        {
            ctx->ReportError("Plugin not found!");
            return -1;
        }
    }

    std::shared_ptr<Forward> plForward = fwdMngr->createForwardCore(fwdName,
                                                                    execType,
                                                                    fwdParamsList,
                                                                    fwdParamsNum,
                                                                    plugin);
    if (!plForward)
        return -1;

    return plForward->getId();
}

// native bool pushCell(any cell)
static cell_t pushCell(SourcePawn::IPluginContext *ctx,
                       const cell_t *params)
{
    cell_t fwdId = params[1];
    if (fwdId == -1)
    {
        ctx->ReportError("Invalid forward!");
        return 0;
    }
    
    const std::unique_ptr<ForwardMngr> &fwdMngr = gSPGlobal->getForwardManagerCore();
    std::shared_ptr<Forward> forward = fwdMngr->findForwardCore(fwdId);
    if (!forward)
    {
        ctx->ReportError("Forward not found!");
        return 0;
    }

    return forward->pushCell(params[2]);
}

// native bool pushCellRef(any &cell)
static cell_t pushCellRef(SourcePawn::IPluginContext *ctx,
                          const cell_t *params)
{
    cell_t fwdId = params[1];
    if (fwdId == -1)
    {
        ctx->ReportError("Invalid forward!");
        return 0;
    }

    const std::unique_ptr<ForwardMngr> &fwdMngr = gSPGlobal->getForwardManagerCore();
    std::shared_ptr<Forward> forward = fwdMngr->findForwardCore(fwdId);
    if (!forward)
    {
        ctx->ReportError("Forward not found!");
        return 0;
    }

    cell_t *value;
    ctx->LocalToPhysAddr(params[2], &value);
    return forward->pushCellPtr(value, true);
}

// native bool pushFloat(float real)
static cell_t pushFloat(SourcePawn::IPluginContext *ctx,
                        const cell_t *params)
{
    cell_t fwdId = params[1];
    if (fwdId == -1)
    {
        ctx->ReportError("Invalid forward!");
        return 0;
    }

    const std::unique_ptr<ForwardMngr> &fwdMngr = gSPGlobal->getForwardManagerCore();
    std::shared_ptr<Forward> forward = fwdMngr->findForwardCore(fwdId);
    if (!forward)
    {
        ctx->ReportError("Forward not found!");
        return 0;
    }

    return forward->pushFloat(sp_ctof(params[2]));
}

// native bool pushFloatRef(float &real)
static cell_t pushFloatRef(SourcePawn::IPluginContext *ctx,
                           const cell_t *params)
{
    cell_t fwdId = params[1];
    if (fwdId == -1)
    {
        ctx->ReportError("Invalid forward!");
        return 0;
    }

    const std::unique_ptr<ForwardMngr> &fwdMngr = gSPGlobal->getForwardManagerCore();
    std::shared_ptr<Forward> forward = fwdMngr->findForwardCore(fwdId);
    if (!forward)
    {
        ctx->ReportError("Forward not found!");
        return 0;
    }

    cell_t *real;
    ctx->LocalToPhysAddr(params[2], &real);
    return forward->pushFloatPtr(*reinterpret_cast<float **>(&real), true);
}

// native bool pushString(const char[] string)
static cell_t pushString(SourcePawn::IPluginContext *ctx,
                         const cell_t *params)
{
    cell_t fwdId = params[1];
    if (fwdId == -1)
    {
        ctx->ReportError("Invalid forward!");
        return 0;
    }

    const std::unique_ptr<ForwardMngr> &fwdMngr = gSPGlobal->getForwardManagerCore();
    std::shared_ptr<Forward> forward = fwdMngr->findForwardCore(fwdId);
    if (!forward)
    {
        ctx->ReportError("Forward not found!");
        return 0;
    }

    char *string;
    ctx->LocalToString(params[2], &string);
    return forward->pushString(string);
}

// native bool pushArray(const any[] array, int size)
static cell_t pushArray(SourcePawn::IPluginContext *ctx,
                        const cell_t *params)
{
    cell_t fwdId = params[1];
    if (fwdId == -1)
    {
        ctx->ReportError("Invalid forward!");
        return 0;
    }

    const std::unique_ptr<ForwardMngr> &fwdMngr = gSPGlobal->getForwardManagerCore();
    std::shared_ptr<Forward> forward = fwdMngr->findForwardCore(fwdId);
    if (!forward)
    {
        ctx->ReportError("Forward not found!");
        return 0;
    }

    cell_t *array;
    ctx->LocalToPhysAddr(params[2], &array);
    return forward->pushArray(array, params[3], false);
}

// native bool pushStringEx(char[] string, int length, StringFlags sflags, bool copyback)
static cell_t pushStringEx(SourcePawn::IPluginContext *ctx,
                           const cell_t *params)
{
    cell_t fwdId = params[1];
    if (fwdId == -1)
    {
        ctx->ReportError("Invalid forward!");
        return 0;
    }

    const std::unique_ptr<ForwardMngr> &fwdMngr = gSPGlobal->getForwardManagerCore();
    std::shared_ptr<Forward> forward = fwdMngr->findForwardCore(fwdId);
    if (!forward)
    {
        ctx->ReportError("Forward not found!");
        return 0;
    }

    char *string;
    ctx->LocalToString(params[2], &string);
    auto sflags = static_cast<IForward::StringFlags>(params[4]);
    return forward->pushStringEx(string, params[3], sflags, params[5]);
}

// native bool pushArrayEx(any[] array, int size, int cpflags)
static cell_t pushArrayEx(SourcePawn::IPluginContext *ctx,
                          const cell_t *params)
{
    cell_t fwdId = params[1];
    if (fwdId == -1)
    {
        ctx->ReportError("Invalid forward!");
        return 0;
    }

    const std::unique_ptr<ForwardMngr> &fwdMngr = gSPGlobal->getForwardManagerCore();
    std::shared_ptr<Forward> forward = fwdMngr->findForwardCore(fwdId);
    if (!forward)
    {
        ctx->ReportError("Forward not found!");
        return 0;
    }

    cell_t *array;
    ctx->LocalToPhysAddr(params[2], &array);
    return forward->pushArray(array, params[3], params[4]);
}

// native bool pushExec(any &result = 0)
static cell_t pushExec(SourcePawn::IPluginContext *ctx,
                       const cell_t *params)
{
    cell_t fwdId = params[1];
    if (fwdId == -1)
    {
        ctx->ReportError("Invalid forward!");
        return 0;
    }

    const std::unique_ptr<ForwardMngr> &fwdMngr = gSPGlobal->getForwardManagerCore();
    std::shared_ptr<Forward> forward = fwdMngr->findForwardCore(fwdId);
    if (!forward)
    {
        ctx->ReportError("Forward not found!");
        return 0;
    }

    cell_t *retResult;
    ctx->LocalToPhysAddr(params[2], &retResult);

    return forward->execFunc(retResult);
}

// native bool pushCancel()
static cell_t pushCancel(SourcePawn::IPluginContext *ctx,
                         const cell_t *params)
{
    cell_t fwdId = params[1];
    if (fwdId == -1)
    {
        ctx->ReportError("Invalid forward!");
        return 0;
    }

    const std::unique_ptr<ForwardMngr> &fwdMngr = gSPGlobal->getForwardManagerCore();
    std::shared_ptr<Forward> forward = fwdMngr->findForwardCore(fwdId);
    if (!forward)
    {
        ctx->ReportError("Forward not found!");
        return 0;
    }

    forward->resetParams();
    return 1;
}

// native bool remove()
static cell_t forwardRemove(SourcePawn::IPluginContext *ctx,
                            const cell_t *params)
{
    cell_t fwdId = params[1];
    if (fwdId == -1)
    {
        ctx->ReportError("Invalid forward!");
        return 0;
    }

    const std::unique_ptr<ForwardMngr> &fwdMngr = gSPGlobal->getForwardManagerCore();
    std::shared_ptr<Forward> forward = fwdMngr->findForwardCore(fwdId);

    if (!forward)
        return 0;

    if (forward->isExecuted())
        return 0;

    fwdMngr->deleteForwardCore(forward);
    return 1;
}

// native Forward findForward(const char[] name)
static cell_t findForward(SourcePawn::IPluginContext *ctx,
                          const cell_t *params)
{
    char *fwdName;
    ctx->LocalToString(params[1], &fwdName);

    const std::unique_ptr<ForwardMngr> &fwdMngr = gSPGlobal->getForwardManagerCore();
    std::shared_ptr<Forward> forward = fwdMngr->findForwardCore(fwdName);

    if (!forward)
        return -1;

    return forward->getId();
}

sp_nativeinfo_t gForwardsNatives[] =
{
    {  "Forward.Forward",        forwardCtor         },
    {  "Forward.pushCell",       pushCell            },
    {  "Forward.pushCellRef",    pushCellRef         },
    {  "Forward.pushFloat",      pushFloat           },
    {  "Forward.pushFloatRef",   pushFloatRef        },
    {  "Forward.pushString",     pushString          },
    {  "Forward.pushArray",      pushArray           },
    {  "Forward.pushStringEx",   pushStringEx        },
    {  "Forward.pushArrayEx",    pushArrayEx         },
    {  "Forward.pushExec",       pushExec            },
    {  "Forward.pushCancel",     pushCancel          },
    {  "Forward.remove",         forwardRemove       },
    {  "findForward",            findForward         },
    {  nullptr,                  nullptr             }
};
