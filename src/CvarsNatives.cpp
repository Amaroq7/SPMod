/*
 *  Copyright (C) 2018 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
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

static cell_t CvarRegister(SourcePawn::IPluginContext *ctx, 
                           const cell_t *params)
{	
    enum { arg_name = 1, arg_value, arg_flags };
    const std::unique_ptr<CvarMngr> &cvarMngr = gSPGlobal->getCvarManagerCore();
    char *cvarName, *cvarValue;
    ctx->LocalToString(params[arg_name], &cvarName);
    ctx->LocalToString(params[arg_value], &cvarValue);

    auto plCvar = cvarMngr->registerCvarCore(cvarName, cvarValue, static_cast<ICvar::Flags>(params[arg_flags]));
    if (!plCvar)
        return -1;
    
    return plCvar->getId();
}

static cell_t CvarGetName(SourcePawn::IPluginContext *ctx,
                          const cell_t *params)
{
    enum { arg_index = 1, arg_buffer, arg_size };
    char *destBuffer;

    cell_t cvarId = params[arg_index];
    if (cvarId  < 0)
    {
        ctx->ReportError("Invalid cvar pointer!");
        return 0;
    }
    const std::unique_ptr<CvarMngr> &cvarMngr = gSPGlobal->getCvarManagerCore();
    auto cvar = cvarMngr->findCvarCore(cvarId);
    if (!cvar)
    {
        ctx->ReportError("Cvar not found!");
        return 0;
    }

    ctx->LocalToString(params[arg_buffer], &destBuffer);

    return gSPGlobal->getUtilsCore()->strCopyCore(destBuffer, params[arg_size], cvar->getNameCore());
}


static cell_t CvarGetFloat(SourcePawn::IPluginContext *ctx,
                           const cell_t *params)
{
    enum { arg_index = 1 };
    cell_t cvarId = params[arg_index];
    if (cvarId < 0)
    {
        ctx->ReportError("Invalid cvar pointer!");
        return 0;
    }
    const std::unique_ptr<CvarMngr> &cvarMngr = gSPGlobal->getCvarManagerCore();
    auto cvar = cvarMngr->findCvarCore(cvarId);
    if (!cvar)
    {
        ctx->ReportError("Cvar not found!");
        return 0;
    }
    
    return sp_ftoc(cvar->asFloat());
}

static cell_t CvarGetString(SourcePawn::IPluginContext *ctx,
                            const cell_t *params)
{    
    enum { arg_index = 1, arg_buffer, arg_size };
    char *destBuffer;

    cell_t cvarId = params[arg_index];
    if (cvarId  < 0)
    {
        ctx->ReportError("Invalid cvar pointer!");
        return 0;
    }
    const std::unique_ptr<CvarMngr> &cvarMngr = gSPGlobal->getCvarManagerCore();
    auto cvar = cvarMngr->findCvarCore(cvarId);
    if (!cvar)
    {
        ctx->ReportError("Cvar not found!");
        return 0;
    }

    ctx->LocalToString(params[arg_buffer], &destBuffer);

    return gSPGlobal->getUtilsCore()->strCopyCore(destBuffer, params[arg_size], cvar->asStringCore());
}

static cell_t CvarGetInt(SourcePawn::IPluginContext *ctx,
                         const cell_t *params)
{
    enum { arg_index = 1 };
    cell_t cvarId = params[arg_index];
    if (cvarId < 0)
    {
        ctx->ReportError("Invalid cvar pointer!");
        return 0;
    }
    const std::unique_ptr<CvarMngr> &cvarMngr = gSPGlobal->getCvarManagerCore();
    auto cvar = cvarMngr->findCvarCore(cvarId);
    if (!cvar)
    {
        ctx->ReportError("Cvar not found!");
        return 0;
    }

    return cvar->asInt();
}

static cell_t CvarGetFlags(SourcePawn::IPluginContext *ctx,
                           const cell_t *params)
{
    enum { arg_index = 1 };
    cell_t cvarId = params[arg_index];
    if (cvarId  < 0)
    {
        ctx->ReportError("Invalid cvar pointer!");
        return 0;
    }
    const std::unique_ptr<CvarMngr> &cvarMngr = gSPGlobal->getCvarManagerCore();
    auto cvar = cvarMngr->findCvarCore(cvarId);
    if (!cvar)
    {
        ctx->ReportError("Cvar not found!");
        return 0;
    }

    return static_cast<cell_t>(cvar->getFlags());
}

static cell_t CvarSetFloat(SourcePawn::IPluginContext *ctx,
                           const cell_t *params)
{
    enum { arg_index = 1, arg_value };
    cell_t cvarId = params[arg_index];
    if (cvarId < 0)
    {
        ctx->ReportError("Invalid cvar pointer!");
        return 0;
    }
    const std::unique_ptr<CvarMngr> &cvarMngr = gSPGlobal->getCvarManagerCore();
    auto cvar = cvarMngr->findCvarCore(cvarId);
    if (!cvar)
    {
        ctx->ReportError("Cvar not found!");
        return 0;
    }
    cvar->setValue(sp_ctof(params[arg_value]));

    return 1;
}

static cell_t CvarSetString(SourcePawn::IPluginContext *ctx,
                            const cell_t *params)
{
    enum { arg_index = 1, arg_value };
    char *cvarValue;
    ctx->LocalToString(params[arg_value], &cvarValue);
    
    cell_t cvarId = params[arg_index];
    if (cvarId < 0)
    {
        ctx->ReportError("Invalid cvar pointer!");
        return 0;
    }
    const std::unique_ptr<CvarMngr> &cvarMngr = gSPGlobal->getCvarManagerCore();
    auto cvar = cvarMngr->findCvarCore(cvarId);
    if (!cvar)
    {
        ctx->ReportError("Cvar not found!");
        return 0;
    }
    cvar->setValue(cvarValue);

    return 1;
}

static cell_t CvarSetInt(SourcePawn::IPluginContext *ctx,
                         const cell_t *params)
{
    enum { arg_index = 1, arg_value };
    cell_t cvarId = params[arg_index];
    if (cvarId  < 0)
    {
        ctx->ReportError("Invalid cvar pointer!");
        return 0;
    }
    const std::unique_ptr<CvarMngr> &cvarMngr = gSPGlobal->getCvarManagerCore();
    auto cvar = cvarMngr->findCvarCore(cvarId);
    if (!cvar)
    {
        ctx->ReportError("Cvar not found!");
        return 0;
    }
    cvar->setValue(params[arg_value]);

    return 1;
}

static cell_t CvarAddCallback(SourcePawn::IPluginContext *ctx,
                              const cell_t *params)
{
    enum { arg_index = 1, cvar_callback };
    cell_t cvarId = params[arg_index];
    if (cvarId  < 0)
    {
        ctx->ReportError("Invalid cvar pointer!");
        return 0;
    }
    const std::unique_ptr<CvarMngr> &cvarMngr = gSPGlobal->getCvarManagerCore();
    auto cvar = cvarMngr->findCvarCore(cvarId);
    if (!cvar)
    {
        ctx->ReportError("Cvar not found!");
        return 0;
    }
    auto ptr = ctx->GetFunctionById(params[cvar_callback]);
    if (ptr)
    {
        cvar->addPluginCallback(ptr);
    }

    return 1;
}

static cell_t CvarSetFlags(SourcePawn::IPluginContext *ctx,
                           const cell_t *params)
{
    enum { arg_index = 1, arg_flags };
    cell_t cvarId = params[arg_index];
    if (cvarId  < 0)
    {
        ctx->ReportError("Invalid cvar pointer!");
        return 0;
    }
    const std::unique_ptr<CvarMngr> &cvarMngr = gSPGlobal->getCvarManagerCore();
    auto cvar = cvarMngr->findCvarCore(cvarId);
    if (!cvar)
    {
        ctx->ReportError("Cvar not found!");
        return 0;
    }
    cvar->setFlags(static_cast<ICvar::Flags>(params[arg_flags]));

    return 1;
}

static cell_t CvarFind(SourcePawn::IPluginContext *ctx,
                       const cell_t *params)
{
    enum { arg_name = 1 };
    const std::unique_ptr<CvarMngr> &cvarMngr = gSPGlobal->getCvarManagerCore();
    char *cvarName;
    ctx->LocalToString(params[arg_name], &cvarName);

    auto plCvar = cvarMngr->findCvarCore(cvarName, false);

    if (!plCvar)
        return -1;

    return plCvar->getId();
}

sp_nativeinfo_t gCvarsNatives[] =
{
    { "Cvar.Cvar",              CvarRegister    },
    { "Cvar.GetName",           CvarGetName     },
    { "Cvar.GetFloat",          CvarGetFloat    },
    { "Cvar.GetString",         CvarGetString   },
    { "Cvar.GetInt",            CvarGetInt      },
    { "Cvar.GetFlags",          CvarGetFlags    },
    { "Cvar.SetFloat",          CvarSetFloat    },
    { "Cvar.SetString",         CvarSetString   },
    { "Cvar.SetInt",            CvarSetInt      },
    { "Cvar.SetFlags",          CvarSetFlags    },

    { "Cvar.FloatValue.get",    CvarGetFloat    },
    { "Cvar.FloatValue.set",    CvarSetFloat    },
    { "Cvar.IntValue.get",      CvarGetInt      },
    { "Cvar.IntValue.set",      CvarSetInt      },
    { "Cvar.Flags.get",         CvarGetFlags    },
    { "Cvar.Flags.set",         CvarSetFlags    },

    { "Cvar.AddHookOnChange",   CvarAddCallback },

    { "FindCvar",               CvarFind        },
    { nullptr,                  nullptr         }
};
