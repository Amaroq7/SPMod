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

// native void printToServer(const char[] text, any ...)
static cell_t printToServer(SourcePawn::IPluginContext *ctx,
                                 const cell_t *params)
{
    char *formatString;
    char bufferOutput[1024];

    ctx->LocalToString(params[1], &formatString);
    gSPGlobal->formatString(bufferOutput, sizeof(bufferOutput), formatString, ctx, params, 2);

    SERVER_PRINT(bufferOutput);

    return 1;
}

// native int precacheModel(const char[] model)
static cell_t precacheModel(SourcePawn::IPluginContext *ctx,
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

// native int precacheSound(const char[] sound)
static cell_t precacheSound(SourcePawn::IPluginContext *ctx,
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

// native int precacheGeneric(const char[] generic)
static cell_t precacheGeneric(SourcePawn::IPluginContext *ctx,
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

// native int numToString(int num, char[] buffer, int size)
static cell_t numToString(SourcePawn::IPluginContext *ctx,
                                    const cell_t *params)
{
    auto numToConvert = params[1];
    auto numConverted = std::to_string(numToConvert);
    ctx->StringToLocal(params[2], params[3], numConverted.c_str());

    return numConverted.length();
}

// native int realToString(float real, char[] buffer, int size)
static cell_t realToString(SourcePawn::IPluginContext *ctx,
                                    const cell_t *params)
{
    auto realToConvert = sp_ctof(params[1]);
    auto realConverted = std::to_string(realToConvert);
    ctx->StringToLocal(params[2], params[3], realConverted.c_str());

    return realConverted.length();
}

static cell_t copyString(SourcePawn::IPluginContext *ctx,
                              const cell_t *params)
{
    char *destArray, *stringToCopy;
    size_t arraySize = params[2];

    ctx->LocalToString(params[1], &destArray);
    ctx->LocalToString(params[3], &stringToCopy);

    std::strncpy(destArray, stringToCopy, arraySize);

    destArray[arraySize - 1] = '\0';

    return 1;
}

// native bool nativeRegister(const char[] name, PluginNative func)
static cell_t nativeRegister(SourcePawn::IPluginContext *ctx,
                                  const cell_t *params)
{
    char *nativeName, *pluginIdentity;
    ctx->LocalToString(params[1], &nativeName);
    ctx->GetKey(1, reinterpret_cast<void **>(&pluginIdentity));

    SourcePawn::IPluginFunction *fnToExecute = ctx->GetFunctionById(params[2]);

    return gSPGlobal->getNativeManagerCore()->addFakeNative(pluginIdentity, nativeName, fnToExecute);
}

// native any nativeGetCell(int param)
static cell_t nativeGetCell(SourcePawn::IPluginContext *ctx,
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

// native any nativeGetCellRef(int param)
static cell_t nativeGetCellRef(SourcePawn::IPluginContext *ctx,
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

// native int nativeGetString(int param, char[] buffer, int size)
static cell_t nativeGetString(SourcePawn::IPluginContext *ctx,
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

    size_t writtenBytes;
    ctx->StringToLocalUTF8(params[2], params[3], stringToCopy, &writtenBytes);

    return writtenBytes;
}

// native bool nativeGetArray(int param, any[] buffer, int size)
static cell_t nativeGetArray(SourcePawn::IPluginContext *ctx,
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

// native bool nativeSetCellRef(int param, any value)
static cell_t nativeSetCellRef(SourcePawn::IPluginContext *ctx,
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

// native int nativeSetString(int param, const char[] string, int size)
static cell_t nativeSetString(SourcePawn::IPluginContext *ctx,
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

    size_t writtenBytes;
    NativeMngr::m_callerPlugin->StringToLocalUTF8(NativeMngr::m_callerParams[param],
                                                  params[3],
                                                  stringToCopy,
                                                  &writtenBytes);

    return writtenBytes;
}

// native bool nativeSetArray(int param, const any[] buffer, int size)
static cell_t nativeSetArray(SourcePawn::IPluginContext *ctx,
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

sp_nativeinfo_t gCoreNatives[] =
{
    {  "printToServer",          printToServer       },
    {  "precacheModel",          precacheModel       },
    {  "precacheSound",          precacheSound       },
    {  "precacheGeneric",        precacheGeneric     },
    {  "numToString",            numToString         },
    {  "realToString",           realToString        },
    {  "copyString",             copyString          },
    {  "nativeRegister",         nativeRegister      },
    {  "nativeGetCell",          nativeGetCell       },
    {  "nativeGetCellRef",       nativeGetCellRef    },
    {  "nativeGetString",        nativeGetString     },
    {  "nativeGetArray",         nativeGetArray      },
    {  "nativeSetCellRef",       nativeSetCellRef    },
    {  "nativeSetString",        nativeSetString     },
    {  "nativeSetArray",         nativeSetArray      },
    {  nullptr,                  nullptr             }
};
