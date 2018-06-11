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

// native float cvarGetFloat(const char[] name)
static cell_t core_cvarGetFloat(SourcePawn::IPluginContext *ctx,
                                const cell_t *params)
{
    char *cvarName;
    ctx->LocalToString(params[1], &cvarName);

    return sp_ftoc(CVAR_GET_FLOAT(cvarName));
}

// native void cvarGetString(const char[] name, char[] value, int size)
static cell_t core_cvarGetString(SourcePawn::IPluginContext *ctx,
                                 const cell_t *params)
{
    char *cvarName, *destBuffer;
    size_t bufferSize = params[3];

    ctx->LocalToString(params[1], &cvarName);
    ctx->LocalToString(params[2], &destBuffer);

    const char *value = CVAR_GET_STRING(cvarName);

#if defined __STDC_LIB_EXT1__
    #if defined SP_WINDOWS
    strncpy_s(destBuffer, bufferSize, value, _TRUNCATE);
    #else
    strncpy_s(destBuffer, bufferSize, value, bufferSize - 1);
    #endif
#else
    std::strncpy(destBuffer, value, bufferSize);
    destBuffer[bufferSize - 1] = '\0';
#endif

    return 1;
}

// native int cvarGetInt(const char[] name)
static cell_t core_cvarGetInt(SourcePawn::IPluginContext *ctx,
                              const cell_t *params)
{
    char *cvarName;
    ctx->LocalToString(params[1], &cvarName);

    return CVAR_GET_FLOAT(cvarName);
}

// native CvarFlags cvarGetFlags(const char[] name)
static cell_t core_cvarGetFlags(SourcePawn::IPluginContext *ctx,
                                const cell_t *params)
{
    char *cvarName;
    ctx->LocalToString(params[1], &cvarName);

    cvar_t *cvar = CVAR_GET_POINTER(cvarName);
    if (!cvar)
        return 0;

    return cvar->flags;
}

// native void cvarSetFloat(const char[] name, float value)
static cell_t core_cvarSetFloat(SourcePawn::IPluginContext *ctx,
                                const cell_t *params)
{
    char *cvarName;
    ctx->LocalToString(params[1], &cvarName);

    CVAR_SET_FLOAT(cvarName, sp_ctof(params[2]));

    return 1;
}

// native void cvarSetString(const char[] name, const char[] value)
static cell_t core_cvarSetString(SourcePawn::IPluginContext *ctx,
                                 const cell_t *params)
{
    char *cvarName, *cvarValue;
    ctx->LocalToString(params[1], &cvarName);
    ctx->LocalToString(params[2], &cvarValue);

    CVAR_SET_STRING(cvarName, cvarValue);

    return 1;
}

// native void cvarSetInt(const char[] name, int value)
static cell_t core_cvarSetInt(SourcePawn::IPluginContext *ctx,
                              const cell_t *params)
{
    char *cvarName;
    ctx->LocalToString(params[1], &cvarName);

    CVAR_SET_FLOAT(cvarName, params[2]);

    return 1;
}

// native void cvarSetFlags(const char[] name, CvarFlags flags)
static cell_t core_cvarSetFlags(SourcePawn::IPluginContext *ctx,
                                const cell_t *params)
{
    char *cvarName;
    ctx->LocalToString(params[1], &cvarName);

    cvar_t *cvar = CVAR_GET_POINTER(cvarName);
    if (!cvar)
        return 0;

    cvar->flags = params[2];

    return 1;
}

// native void cvarRegister(const char[] name, const char[] value, CvarFlags flags = None)
static cell_t core_cvarRegister(SourcePawn::IPluginContext *ctx,
                                const cell_t *params)
{
    char *cvarName, *cvarValue;
    ctx->LocalToString(params[1], &cvarName);
    ctx->LocalToString(params[2], &cvarValue);

    static cvar_t cvarToRegister;
    cvarToRegister.name = cvarName;
    cvarToRegister.string = cvarValue;
    cvarToRegister.flags = params[3];

    CVAR_REGISTER(&cvarToRegister);

    return 1;
}

sp_nativeinfo_t gCvarsNatives[] =
{
    { "cvarGetFloat",       core_cvarGetFloat       },
    { "cvarGetString",      core_cvarGetString      },
    { "cvarGetInt",         core_cvarGetInt         },
    { "cvarGetFlags",       core_cvarGetFlags       },
    { "cvarSetFloat",       core_cvarSetFloat       },
    { "cvarSetString",      core_cvarSetString      },
    { "cvarSetInt",         core_cvarSetInt         },
    { "cvarSetFlags",       core_cvarSetFlags       },
    { "cvarRegister",       core_cvarRegister       },
    { nullptr,              nullptr                 }
};
