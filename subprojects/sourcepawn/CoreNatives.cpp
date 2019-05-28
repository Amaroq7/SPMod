/*
 *  Copyright (C) 2018 SPMod Development Team
 *
 *  This file is part of SPMod.
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

#include "ExtMain.hpp"

// native void PrintToServer(const char[] text, any ...)
static cell_t PrintToServer(SourcePawn::IPluginContext *ctx,
                            const cell_t *params)
{
    char *formatString;
    char bufferOutput[1024];

    ctx->LocalToString(params[1], &formatString);
    std::size_t res = formatString(bufferOutput, sizeof(bufferOutput) - 2, formatString, ctx, params, 2);

    bufferOutput[res++] = '\n';
    bufferOutput[res] = '\0';

    SERVER_PRINT(bufferOutput);

    return 1;
}

// native int PrecacheModel(const char[] model)
static cell_t PrecacheModel(SourcePawn::IPluginContext *ctx,
                            const cell_t *params)
{
    if (!gSPGlobal->canPluginPrecache())
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
    if (!gSPGlobal->canPluginPrecache())
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
    if (!gSPGlobal->canPluginPrecache())
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
    char *nativeName;
    ctx->LocalToString(params[1], &nativeName);
    std::shared_ptr<Plugin> plugin = gModuleInterface->getPluginMngrCore()->getPluginCore(ctx);

    return gSPGlobal->getNativeProxy()->registerNative(nativeName, ctx->GetFunctionById(params[2]), plugin.get());
}

// native int NativeGetInt(int param)
static cell_t NativeGetInt(SourcePawn::IPluginContext *ctx,
                           const cell_t *params)
{
    if (!PluginMngr::m_callerPlugin)
    {
        ctx->ReportError("No caller plugin");
        return 0;
    }

    cell_t param = params[1];
    int paramsNum = m_callerPlugin->getProxiedParamAsInt(0);

    if (param > paramsNum || param < 0)
    {
        ctx->ReportError("Incorrect parameter %d (range: 0 - %d)", param, paramsNum);
        return 0;
    }

    return PluginMngr::m_callerPlugin->getProxiedParamAsInt(param);
}

// native int NativeGetIntRef(int param)
static cell_t NativeGetIntRef(SourcePawn::IPluginContext *ctx,
                              const cell_t *params)
{
    if (!PluginMngr::m_callerPlugin)
    {
        ctx->ReportError("No caller plugin");
        return 0;
    }

    cell_t param = params[1];
    int paramsNum = m_callerPlugin->getProxiedParamAsInt(0);
    if (param > paramsNum || param < 0)
    {
        ctx->ReportError("Incorrect parameter %d (range: 0 - %d)", param, paramsNum);
        return 0;
    }

    return *PluginMngr::m_callerPlugin->getProxiedParamAsIntAddr(param);
}

// native float NativeGetFloat(int param)
static cell_t NativeGetFloat(SourcePawn::IPluginContext *ctx,
                             const cell_t *params)
{
    if (!PluginMngr::m_callerPlugin)
    {
        ctx->ReportError("No caller plugin");
        return 0;
    }

    cell_t param = params[1];
    int paramsNum = m_callerPlugin->getProxiedParamAsInt(0);

    if (param > paramsNum || param < 0)
    {
        ctx->ReportError("Incorrect parameter %d (range: 0 - %d)", param, paramsNum);
        return 0;
    }

    return sp_ftoc(PluginMngr::m_callerPlugin->getProxiedParamAsFloat(param));
}

// native float NativeGetFloatRef(int param)
static cell_t NativeGetFloatRef(SourcePawn::IPluginContext *ctx,
                                const cell_t *params)
{
    if (!PluginMngr::m_callerPlugin)
    {
        ctx->ReportError("No caller plugin");
        return 0;
    }

    cell_t param = params[1];
    int paramsNum = m_callerPlugin->getProxiedParamAsInt(0);
    if (param > paramsNum || param < 0)
    {
        ctx->ReportError("Incorrect parameter %d (range: 0 - %d)", param, paramsNum);
        return 0;
    }

    return sp_ftoc(*PluginMngr::m_callerPlugin->getProxiedParamAsFloatAddr(param));
}

// native int NativeGetString(int param, char[] buffer, int size)
static cell_t NativeGetString(SourcePawn::IPluginContext *ctx,
                              const cell_t *params)
{
    if (!PluginMngr::m_callerPlugin)
    {
        ctx->ReportError("No caller plugin");
        return 0;
    }

    cell_t param = params[1];
    int paramsNum = m_callerPlugin->getProxiedParamAsInt(0);
    if (param > paramsNum || param < 0)
    {
        ctx->ReportError("Incorrect parameter %d (range: 0 - %d)", param, paramsNum);
        return 0;
    }

    const char *stringToCopy = PluginMngr::m_callerPlugin->getProxiedParamAsString(param);

    std::size_t writtenBytes;
    ctx->StringToLocalUTF8(params[2], params[3], stringToCopy, &writtenBytes);

    return writtenBytes;
}

// native bool NativeGetArrayInt(int param, int[] buffer, int size)
static cell_t NativeGetArrayInt(SourcePawn::IPluginContext *ctx,
                                const cell_t *params)
{
    if (!PluginMngr::m_callerPlugin)
    {
        ctx->ReportError("No caller plugin");
        return 0;
    }

    cell_t param = params[1];
    int paramsNum = m_callerPlugin->getProxiedParamAsInt(0);
    if (param > paramsNum || param < 0)
    {
        ctx->ReportError("Incorrect parameter %d (range: 0 - %d)", param, paramsNum);
        return 0;
    }

    cell_t *destArray;
    ctx->LocalToPhysAddr(params[2], &destArray);
    auto srcArray = reinterpret_cast<cell_t *>(PluginMngr::m_callerPlugin->getProxiedParamAsArray(param));

    std::copy_n(srcArray, params[3], destArray);

    return 1;
}

// native bool NativeGetArrayFloat(int param, float[] buffer, int size)
static cell_t NativeGetArrayFloat(SourcePawn::IPluginContext *ctx,
                                  const cell_t *params)
{
    if (!PluginMngr::m_callerPlugin)
    {
        ctx->ReportError("No caller plugin");
        return 0;
    }

    cell_t param = params[1];
    int paramsNum = m_callerPlugin->getProxiedParamAsInt(0);
    if (param > paramsNum || param < 0)
    {
        ctx->ReportError("Incorrect parameter %d (range: 0 - %d)", param, paramsNum);
        return 0;
    }

    cell_t *destArray;
    ctx->LocalToPhysAddr(params[2], &destArray);
    auto srcArray = reinterpret_cast<float *>(PluginMngr::m_callerPlugin->getProxiedParamAsArray(param));

    for (std::size_t i = 0; i < params[3]; i++)
    {
        destArray[i] = sp_ftoc(srcArray[i]);
    }

    return 1;
}

// native bool NativeSetIntRef(int param, int value)
static cell_t NativeSetIntRef(SourcePawn::IPluginContext *ctx,
                              const cell_t *params)
{
    if (!PluginMngr::m_callerPlugin)
    {
        ctx->ReportError("No caller plugin");
        return 0;
    }

    cell_t param = params[1];
    int paramsNum = m_callerPlugin->getProxiedParamAsInt(0);
    if (param > paramsNum || param < 0)
    {
        ctx->ReportError("Incorrect parameter %d (range: 0 - %d)", param, paramsNum);
        return 0;
    }

    *PluginMngr::m_callerPlugin->getProxiedParamAsIntAddr(param) = params[2];
    return 1;
}

// native bool NativeSetFloatRef(int param, float value)
static cell_t NativeSetFloatRef(SourcePawn::IPluginContext *ctx,
                                const cell_t *params)
{
    if (!PluginMngr::m_callerPlugin)
    {
        ctx->ReportError("No caller plugin");
        return 0;
    }

    cell_t param = params[1];
    int paramsNum = m_callerPlugin->getProxiedParamAsInt(0);
    if (param > paramsNum || param < 0)
    {
        ctx->ReportError("Incorrect parameter %d (range: 0 - %d)", param, paramsNum);
        return 0;
    }

    *PluginMngr::m_callerPlugin->getProxiedParamAsFloatAddr(param) = sp_ctof(params[2]);
    return 1;
}

// native int NativeSetString(int param, const char[] string, int size)
static cell_t NativeSetString(SourcePawn::IPluginContext *ctx,
                              const cell_t *params)
{
    if (!PluginMngr::m_callerPlugin)
    {
        ctx->ReportError("No caller plugin");
        return 0;
    }

    cell_t param = params[1];
    int paramsNum = m_callerPlugin->getProxiedParamAsInt(0);
    if (param > paramsNum || param < 0)
    {
        ctx->ReportError("Incorrect parameter %d (range: 0 - %d)", param, paramsNum);
        return 0;
    }

    char *stringToCopy;
    ctx->LocalToString(params[2], &stringToCopy);

    std::size_t writtenBytes;
    char *destString = PluginMngr::m_callerPlugin->getProxiedParamAsString(param);

    std::copy_n(stringToCopy, params[3], destString);

    return params[3];
}

// native bool NativeSetArrayInt(int param, const int[] buffer, int size)
static cell_t NativeSetArrayInt(SourcePawn::IPluginContext *ctx,
                                const cell_t *params)
{
    if (!PluginMngr::m_callerPlugin)
    {
        ctx->ReportError("No caller plugin");
        return 0;
    }

    cell_t param = params[1];
    int paramsNum = m_callerPlugin->getProxiedParamAsInt(0);
    if (param > paramsNum || param < 0)
    {
        ctx->ReportError("Incorrect parameter %d (range: 0 - %d)", param, paramsNum);
        return 0;
    }

    cell_t *arrayToCopy;
    ctx->LocalToPhysAddr(params[2], &arrayToCopy);
    auto destArray = reinterpret_cast<cell_t *>(PluginMngr::m_callerPlugin->getProxiedParamAsArray(param));

    std::copy_n(arrayToCopy, params[3], destArray);

    return 1;
}

// native bool NativeSetArrayFloat(int param, const float[] buffer, int size)
static cell_t NativeSetArrayFloat(SourcePawn::IPluginContext *ctx,
                                  const cell_t *params)
{
    if (!PluginMngr::m_callerPlugin)
    {
        ctx->ReportError("No caller plugin");
        return 0;
    }

    cell_t param = params[1];
    int paramsNum = m_callerPlugin->getProxiedParamAsInt(0);
    if (param > paramsNum || param < 0)
    {
        ctx->ReportError("Incorrect parameter %d (range: 0 - %d)", param, paramsNum);
        return 0;
    }

    cell_t *arrayToCopy;
    ctx->LocalToPhysAddr(params[2], &arrayToCopy);
    auto destArray = reinterpret_cast<float *>(PluginMngr::m_callerPlugin->getProxiedParamAsArray(param));

    for (std::size_t i = 0; i < params[3]; i++)
    {
        destArray[i] = sp_ctof(arrayToCopy[i]);
    }

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

// native void ServerCmd(const char[] command, any ...)
static cell_t ServerCmd(SourcePawn::IPluginContext *ctx,
                        const cell_t *params)
{
    enum { arg_buffer = 1, arg_params };

    char *formatString;
    char bufferOutput[1024];

    ctx->LocalToString(params[arg_buffer], &formatString);
    std::size_t res = formatString(bufferOutput, sizeof(bufferOutput) - 2, formatString, ctx, params, arg_params);
  
    bufferOutput[res++] = '\n';
    bufferOutput[res] = '\0';
  
    SERVER_COMMAND(bufferOutput);

    return 1;
}

// native void ServerExec()
static cell_t ServerExec(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                         const cell_t *params [[maybe_unused]])
{
    SERVER_EXECUTE();
    return 1;
}

sp_nativeinfo_t gCoreNatives[] =
{
    {  "PrintToServer",          PrintToServer       },
    {  "PrecacheModel",          PrecacheModel       },
    {  "PrecacheSound",          PrecacheSound       },
    {  "PrecacheGeneric",        PrecacheGeneric     },
    {  "NativeRegister",         NativeRegister      },
    {  "NativeGetInt",           NativeGetInt        },
    {  "NativeGetIntRef",        NativeGetIntRef     },
    {  "NativeGetFloat",         NativeGetInt        },
    {  "NativeGetFloatRef",      NativeGetIntRef     },
    {  "NativeGetString",        NativeGetString     },
    {  "NativeGetArray",         NativeGetArray      },
    {  "NativeSetIntRef",        NativeSetIntRef     },
    {  "NativeSetFloatRef",      NativeSetFloatRef   },
    {  "NativeSetString",        NativeSetString     },
    {  "NativeSetArrayInt",      NativeSetArrayInt   },
    {  "NativeSetArrayFloat",    NativeSetArrayFloat },
    {  "ChangeLevel",            ChangeLevel         },
    {  "GetGameTime",            GetGameTime         },
    {  "ServerCmd",              ServerCmd           },
    {  "ServerExec",             ServerExec          },
    {  nullptr,                  nullptr             }
};
