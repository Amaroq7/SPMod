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

// native void PrintToServer(const char[] text, any ...)
static cell_t PrintToServer(SourcePawn::IPluginContext *ctx,
                                 const cell_t *params)
{
    char *formatString;
    char bufferOutput[1024];

    ctx->LocalToString(params[1], &formatString);
    std::size_t res = gSPGlobal->formatString(bufferOutput, sizeof(bufferOutput)-2, formatString, ctx, params, 2);
  
    bufferOutput[res++] = '\n';
    bufferOutput[res] = '\0';
  
    SERVER_PRINT(bufferOutput);

    return 1;
}

// native int PrecacheModel(const char[] model)
static cell_t PrecacheModel(SourcePawn::IPluginContext *ctx,
                                    const cell_t *params)
{
    if (!gSPGlobal->getPluginManagerCore()->canPluginPrecache())
    {
        ctx->ReportError("Precaching is not allowed");
        return 0;
    }

    char *modelToPrecache;
    ctx->LocalToString(params[1], &modelToPrecache);

    return PRECACHE_MODEL(modelToPrecache);
}

// native int PrecacheSound(const char[] sound)
static cell_t PrecacheSound(SourcePawn::IPluginContext *ctx,
                                    const cell_t *params)
{
    if (!gSPGlobal->getPluginManagerCore()->canPluginPrecache())
    {
        ctx->ReportError("Precaching is not allowed");
        return 0;
    }

    char *soundToPrecache;
    ctx->LocalToString(params[1], &soundToPrecache);

    return PRECACHE_SOUND(soundToPrecache);
}

// native int PrecacheGeneric(const char[] generic)
static cell_t PrecacheGeneric(SourcePawn::IPluginContext *ctx,
                                    const cell_t *params)
{
    if (!gSPGlobal->getPluginManagerCore()->canPluginPrecache())
    {
        ctx->ReportError("Precaching is not allowed");
        return 0;
    }

    char *genericToPrecache;
    ctx->LocalToString(params[1], &genericToPrecache);

    return PRECACHE_GENERIC(genericToPrecache);
}

// native bool NativeRegister(const char[] name, PluginNative func)
static cell_t NativeRegister(SourcePawn::IPluginContext *ctx,
                                  const cell_t *params)
{
    char *nativeName, *pluginIdentity;
    ctx->LocalToString(params[1], &nativeName);
    ctx->GetKey(1, reinterpret_cast<void **>(&pluginIdentity));

    SourcePawn::IPluginFunction *fnToExecute = ctx->GetFunctionById(params[2]);

    return gSPGlobal->getNativeManagerCore()->addFakeNative(pluginIdentity, nativeName, fnToExecute);
}

// native any NativeGetCell(int param)
static cell_t NativeGetCell(SourcePawn::IPluginContext *ctx,
                                 const cell_t *params)
{
    cell_t param = params[1];
    if (param > NativeMngr::m_callerParams[0] || param < 0)
    {
        ctx->ReportError("Incorrect parameter! %d (range: 0 - %d)", param, NativeMngr::m_callerParams[0]);
        return 0;
    }
    if (!NativeMngr::m_callerPlugin)
    {
        ctx->ReportError("No caller plugin!");
        return 0;
    }

    return NativeMngr::m_callerParams[param];
}

// native any NativeGetCellRef(int param)
static cell_t NativeGetCellRef(SourcePawn::IPluginContext *ctx,
                                    const cell_t *params)
{
    cell_t param = params[1];
    if (param > NativeMngr::m_callerParams[0] || param < 0)
    {
        ctx->ReportError("Incorrect parameter! %d (range: 0 - %d)", param, NativeMngr::m_callerParams[0]);
        return 0;
    }
    if (!NativeMngr::m_callerPlugin)
    {
        ctx->ReportError("No caller plugin!");
        return 0;
    }

    cell_t *paramToGet;
    NativeMngr::m_callerPlugin->LocalToPhysAddr(NativeMngr::m_callerParams[param], &paramToGet);
    return *paramToGet;
}

// native int NativeGetString(int param, char[] buffer, int size)
static cell_t NativeGetString(SourcePawn::IPluginContext *ctx,
                                   const cell_t *params)
{
    cell_t param = params[1];
    if (param > NativeMngr::m_callerParams[0] || param < 0)
    {
        ctx->ReportError("Incorrect parameter! %d (range: 0 - %d)", param, NativeMngr::m_callerParams[0]);
        return 0;
    }
    if (!NativeMngr::m_callerPlugin)
    {
        ctx->ReportError("No caller plugin!");
        return 0;
    }

    char *stringToCopy;
    NativeMngr::m_callerPlugin->LocalToString(NativeMngr::m_callerParams[param], &stringToCopy);

    std::size_t writtenBytes;
    ctx->StringToLocalUTF8(params[2], params[3], stringToCopy, &writtenBytes);

    return writtenBytes;
}

// native bool NativeGetArray(int param, any[] buffer, int size)
static cell_t NativeGetArray(SourcePawn::IPluginContext *ctx,
                                  const cell_t *params)
{
    cell_t param = params[1];
    if (param > NativeMngr::m_callerParams[0] || param < 0)
    {
        ctx->ReportError("Incorrect parameter! %d (range: 0 - %d)", param, NativeMngr::m_callerParams[0]);
        return 0;
    }
    if (!NativeMngr::m_callerPlugin)
    {
        ctx->ReportError("No caller plugin!");
        return 0;
    }

    cell_t *arrayToCopy, *destArray;
    NativeMngr::m_callerPlugin->LocalToPhysAddr(NativeMngr::m_callerParams[param], &arrayToCopy);
    ctx->LocalToPhysAddr(params[2], &destArray);

    std::copy_n(arrayToCopy, params[3], destArray);

    return 1;
}

// native bool NativeSetCellRef(int param, any value)
static cell_t NativeSetCellRef(SourcePawn::IPluginContext *ctx,
                                    const cell_t *params)
{
    cell_t param = params[1];
    if (param > NativeMngr::m_callerParams[0] || param < 0)
    {
        ctx->ReportError("Incorrect parameter! %d (range: 0 - %d)", param, NativeMngr::m_callerParams[0]);
        return 0;
    }
    if (!NativeMngr::m_callerPlugin)
    {
        ctx->ReportError("No caller plugin!");
        return 0;
    }

    cell_t *paramToSet;
    NativeMngr::m_callerPlugin->LocalToPhysAddr(NativeMngr::m_callerParams[param], &paramToSet);

    *paramToSet = params[2];
    return 1;
}

// native int NativeSetString(int param, const char[] string, int size)
static cell_t NativeSetString(SourcePawn::IPluginContext *ctx,
                                   const cell_t *params)
{
    cell_t param = params[1];
    if (param > NativeMngr::m_callerParams[0] || param < 0)
    {
        ctx->ReportError("Incorrect parameter! %d (range: 0 - %d)", param, NativeMngr::m_callerParams[0]);
        return 0;
    }
    if (!NativeMngr::m_callerPlugin)
    {
        ctx->ReportError("No caller plugin!");
        return 0;
    }

    char *stringToCopy;
    ctx->LocalToString(params[2], &stringToCopy);

    std::size_t writtenBytes;
    NativeMngr::m_callerPlugin->StringToLocalUTF8(NativeMngr::m_callerParams[param],
                                                  params[3],
                                                  stringToCopy,
                                                  &writtenBytes);

    return writtenBytes;
}

// native bool NativeSetArray(int param, const any[] buffer, int size)
static cell_t NativeSetArray(SourcePawn::IPluginContext *ctx,
                                  const cell_t *params)
{
    cell_t param = params[1];
    if (param > NativeMngr::m_callerParams[0] || param < 0)
    {
        ctx->ReportError("Incorrect parameter! %d (range: 0 - %d)", param, NativeMngr::m_callerParams[0]);
        return 0;
    }
    if (!NativeMngr::m_callerPlugin)
    {
        ctx->ReportError("No caller plugin!");
        return 0;
    }

    cell_t *arrayToCopy, *destArray;
    NativeMngr::m_callerPlugin->LocalToPhysAddr(NativeMngr::m_callerParams[param], &destArray);
    ctx->LocalToPhysAddr(params[2], &arrayToCopy);

    std::copy_n(arrayToCopy, params[3], destArray);

    return 1;
}

// void ChangeLevel(const char[] map)
static cell_t ChangeLevel(SourcePawn::IPluginContext *ctx,
                                  const cell_t *params)
{
    enum { arg_map = 1 };

    char *newMap;
    ctx->LocalToString(params[arg_map], &newMap);
    gpEngineFuncs->pfnChangeLevel(newMap, nullptr);

    return 1;
}

// float GetGameTime() 
static cell_t GetGameTime(SourcePawn::IPluginContext *ctx [[maybe_unused]], 
                                  const cell_t *params [[maybe_unused]])
{
    return sp_ftoc(gpGlobals->time);
}

sp_nativeinfo_t gCoreNatives[] =
{
    {  "PrintToServer",          PrintToServer       },
    {  "PrecacheModel",          PrecacheModel       },
    {  "PrecacheSound",          PrecacheSound       },
    {  "PrecacheGeneric",        PrecacheGeneric     },
    {  "NativeRegister",         NativeRegister      },
    {  "NativeGetCell",          NativeGetCell       },
    {  "NativeGetCellRef",       NativeGetCellRef    },
    {  "NativeGetString",        NativeGetString     },
    {  "NativeGetArray",         NativeGetArray      },
    {  "NativeSetCellRef",       NativeSetCellRef    },
    {  "NativeSetString",        NativeSetString     },
    {  "NativeSetArray",         NativeSetArray      },
    {  "ChangeLevel",            ChangeLevel         },
    {  "GetGameTime",            GetGameTime         },
    {  nullptr,                  nullptr             }
};
