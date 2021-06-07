/*
 *  Copyright (C) 2018-2021 SPMod Development Team
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

#include "CoreNatives.hpp"
#include "NativeCallback.hpp"
#include "PrintfImpl.hpp"
#include "StringNatives.hpp"
#include "AdapterInterface.hpp"
#include "PluginSystem.hpp"
#include "ExtMain.hpp"

#include <metamodcpp_sdk/engine/ILibrary.hpp>

std::vector<std::unique_ptr<SPExt::NativeCallback>> gSourcePawnPluginsNatives;
SPMod::INativeProxy *gSPNativeProxy;

namespace SPExt
{
    bool initCoreNatives()
    {
        gSPNativeProxy = gSPGlobal->getNativeProxy();
        return gSPNativeProxy->isVersionCompatible(SPMod::INativeProxy::VERSION);
    }
}

// native void PrintToServer(const char[] text, any ...)
static cell_t PrintToServer(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    char *spFormatString;
    char bufferOutput[1024];

    ctx->LocalToString(params[1], &spFormatString);
    std::size_t res = formatString(bufferOutput, sizeof(bufferOutput) - 2, spFormatString, ctx, params, 2);

    bufferOutput[res++] = '\n';
    bufferOutput[res] = '\0';

    gEngine->print(bufferOutput, Metamod::FuncCallType::Direct);

    return 1;
}

// native int PrecacheModel(const char[] model)
static cell_t PrecacheModel(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    if (!gSPGlobal->canPluginsPrecache())
    {
        ctx->ReportError("Precaching is not allowed");
        return 0;
    }

    char *modelToPrecache;
    ctx->LocalToString(params[1], &modelToPrecache);

    return gEngine->precacheModel(modelToPrecache, Metamod::FuncCallType::Direct);
}

// native int PrecacheSound(const char[] sound)
static cell_t PrecacheSound(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    if (!gSPGlobal->canPluginsPrecache())
    {
        ctx->ReportError("Precaching is not allowed");
        return 0;
    }

    char *soundToPrecache;
    ctx->LocalToString(params[1], &soundToPrecache);

    return gEngine->precacheSound(soundToPrecache, Metamod::FuncCallType::Direct);
}

// native int PrecacheGeneric(const char[] generic)
static cell_t PrecacheGeneric(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    if (!gSPGlobal->canPluginsPrecache())
    {
        ctx->ReportError("Precaching is not allowed");
        return 0;
    }

    char *genericToPrecache;
    ctx->LocalToString(params[1], &genericToPrecache);

    return gEngine->precacheGeneric(genericToPrecache, Metamod::FuncCallType::Direct);
}

// native bool NativeRegister(const char[] name, PluginNative func)
static cell_t NativeRegister(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    char *nativeName;
    ctx->LocalToString(params[1], &nativeName);

    if (!gAdapterInterface->getPluginMngrImpl()->registerNative(nativeName, ctx->GetFunctionById(params[2])))
    {
        ctx->ReportError("Native with the same name already registered");
        return 0;
    }

    return 1;
}

// native int NativeGetInt(int param)
static cell_t NativeGetInt(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    if (SPExt::NativeCallback::currentPluginNative.empty())
    {
        ctx->ReportError("No native executed");
        return 0;
    }

    cell_t param = params[1];
    SPExt::NativeCallback *nativeCallback = SPExt::NativeCallback::currentPluginNative.top();
    std::int32_t paramsNum = nativeCallback->getParamAsInt(0);

    if (param > paramsNum || param < 0)
    {
        ctx->ReportError("Incorrect parameter %d (range: 0 - %d)", param, paramsNum);
        return 0;
    }

    return nativeCallback->getParamAsInt(param);
}

// native int NativeGetIntRef(int param)
static cell_t NativeGetIntRef(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    if (SPExt::NativeCallback::currentPluginNative.empty())
    {
        ctx->ReportError("No native executed");
        return 0;
    }

    cell_t param = params[1];
    SPExt::NativeCallback *nativeCallback = SPExt::NativeCallback::currentPluginNative.top();
    std::int32_t paramsNum = nativeCallback->getParamAsInt(0);
    if (param > paramsNum || param < 0)
    {
        ctx->ReportError("Incorrect parameter %d (range: 0 - %d)", param, paramsNum);
        return 0;
    }

    return *nativeCallback->getParamAsIntAddr(param);
}

// native float NativeGetFloat(int param)
static cell_t NativeGetFloat(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    if (SPExt::NativeCallback::currentPluginNative.empty())
    {
        ctx->ReportError("No caller plugin");
        return 0;
    }

    cell_t param = params[1];
    SPExt::NativeCallback *nativeCallback = SPExt::NativeCallback::currentPluginNative.top();
    std::int32_t paramsNum = nativeCallback->getParamAsInt(0);

    if (param > paramsNum || param < 0)
    {
        ctx->ReportError("Incorrect parameter %d (range: 0 - %d)", param, paramsNum);
        return 0;
    }

    return sp_ftoc(nativeCallback->getParamAsFloat(param));
}

// native float NativeGetFloatRef(int param)
static cell_t NativeGetFloatRef(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    if (SPExt::NativeCallback::currentPluginNative.empty())
    {
        ctx->ReportError("No native executed");
        return 0;
    }

    cell_t param = params[1];
    SPExt::NativeCallback *nativeCallback = SPExt::NativeCallback::currentPluginNative.top();
    std::int32_t paramsNum = nativeCallback->getParamAsInt(0);
    if (param > paramsNum || param < 0)
    {
        ctx->ReportError("Incorrect parameter %d (range: 0 - %d)", param, paramsNum);
        return 0;
    }

    return sp_ftoc(*nativeCallback->getParamAsFloatAddr(param));
}

// native int NativeGetString(int param, char[] buffer, int size)
static cell_t NativeGetString(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    if (SPExt::NativeCallback::currentPluginNative.empty())
    {
        ctx->ReportError("No native executed");
        return 0;
    }

    cell_t param = params[1];
    SPExt::NativeCallback *nativeCallback = SPExt::NativeCallback::currentPluginNative.top();
    std::int32_t paramsNum = nativeCallback->getParamAsInt(0);
    if (param > paramsNum || param < 0)
    {
        ctx->ReportError("Incorrect parameter %d (range: 0 - %d)", param, paramsNum);
        return 0;
    }

    const char *stringToCopy = nativeCallback->getParamAsString(param);

    std::size_t writtenBytes;
    ctx->StringToLocalUTF8(params[2], params[3], stringToCopy, &writtenBytes);

    return writtenBytes;
}

// native bool NativeGetArrayInt(int param, int[] buffer, int size)
static cell_t NativeGetArrayInt(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    if (SPExt::NativeCallback::currentPluginNative.empty())
    {
        ctx->ReportError("No native executed");
        return 0;
    }

    cell_t param = params[1];
    SPExt::NativeCallback *nativeCallback = SPExt::NativeCallback::currentPluginNative.top();
    std::int32_t paramsNum = nativeCallback->getParamAsInt(0);
    if (param > paramsNum || param < 0)
    {
        ctx->ReportError("Incorrect parameter %d (range: 0 - %d)", param, paramsNum);
        return 0;
    }

    cell_t *destArray;
    ctx->LocalToPhysAddr(params[2], &destArray);
    auto srcArray = reinterpret_cast<cell_t *>(nativeCallback->getParamAsArray(param));

    std::copy_n(srcArray, params[3], destArray);

    return 1;
}

// native bool NativeGetArrayFloat(int param, float[] buffer, int size)
static cell_t NativeGetArrayFloat(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    if (SPExt::NativeCallback::currentPluginNative.empty())
    {
        ctx->ReportError("No native executed");
        return 0;
    }

    cell_t param = params[1];
    SPExt::NativeCallback *nativeCallback = SPExt::NativeCallback::currentPluginNative.top();
    std::int32_t paramsNum = nativeCallback->getParamAsInt(0);
    if (param > paramsNum || param < 0)
    {
        ctx->ReportError("Incorrect parameter %d (range: 0 - %d)", param, paramsNum);
        return 0;
    }

    cell_t *destArray;
    ctx->LocalToPhysAddr(params[2], &destArray);
    auto srcArray = reinterpret_cast<float *>(nativeCallback->getParamAsArray(param));

    for (std::size_t i = 0; i < static_cast<std::size_t>(params[3]); i++)
    {
        destArray[i] = sp_ftoc(srcArray[i]);
    }

    return 1;
}

// native bool NativeSetIntRef(int param, int value)
static cell_t NativeSetIntRef(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    if (SPExt::NativeCallback::currentPluginNative.empty())
    {
        ctx->ReportError("No native executed");
        return 0;
    }

    cell_t param = params[1];
    SPExt::NativeCallback *nativeCallback = SPExt::NativeCallback::currentPluginNative.top();
    std::int32_t paramsNum = nativeCallback->getParamAsInt(0);
    if (param > paramsNum || param < 0)
    {
        ctx->ReportError("Incorrect parameter %d (range: 0 - %d)", param, paramsNum);
        return 0;
    }

    *nativeCallback->getParamAsIntAddr(param) = params[2];
    return 1;
}

// native bool NativeSetFloatRef(int param, float value)
static cell_t NativeSetFloatRef(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    if (SPExt::NativeCallback::currentPluginNative.empty())
    {
        ctx->ReportError("No native executed");
        return 0;
    }

    cell_t param = params[1];
    SPExt::NativeCallback *nativeCallback = SPExt::NativeCallback::currentPluginNative.top();
    std::int32_t paramsNum = nativeCallback->getParamAsInt(0);
    if (param > paramsNum || param < 0)
    {
        ctx->ReportError("Incorrect parameter %d (range: 0 - %d)", param, paramsNum);
        return 0;
    }

    *nativeCallback->getParamAsFloatAddr(param) = sp_ctof(params[2]);
    return 1;
}

// native int NativeSetString(int param, const char[] string, int size)
static cell_t NativeSetString(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    if (SPExt::NativeCallback::currentPluginNative.empty())
    {
        ctx->ReportError("No native executed");
        return 0;
    }

    cell_t param = params[1];
    SPExt::NativeCallback *nativeCallback = SPExt::NativeCallback::currentPluginNative.top();
    std::int32_t paramsNum = nativeCallback->getParamAsInt(0);
    if (param > paramsNum || param < 0)
    {
        ctx->ReportError("Incorrect parameter %d (range: 0 - %d)", param, paramsNum);
        return 0;
    }

    char *stringToCopy;
    ctx->LocalToString(params[2], &stringToCopy);

    char *destString = nativeCallback->getParamAsString(param);
    std::copy_n(stringToCopy, params[3], destString);

    return params[3];
}

// native bool NativeSetArrayInt(int param, const int[] buffer, int size)
static cell_t NativeSetArrayInt(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    if (SPExt::NativeCallback::currentPluginNative.empty())
    {
        ctx->ReportError("No native executed");
        return 0;
    }

    cell_t param = params[1];
    SPExt::NativeCallback *nativeCallback = SPExt::NativeCallback::currentPluginNative.top();
    std::int32_t paramsNum = nativeCallback->getParamAsInt(0);
    if (param > paramsNum || param < 0)
    {
        ctx->ReportError("Incorrect parameter %d (range: 0 - %d)", param, paramsNum);
        return 0;
    }

    cell_t *arrayToCopy;
    ctx->LocalToPhysAddr(params[2], &arrayToCopy);
    auto destArray = reinterpret_cast<cell_t *>(nativeCallback->getParamAsArray(param));

    std::copy_n(arrayToCopy, params[3], destArray);

    return 1;
}

// native bool NativeSetArrayFloat(int param, const float[] buffer, int size)
static cell_t NativeSetArrayFloat(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    if (SPExt::NativeCallback::currentPluginNative.empty())
    {
        ctx->ReportError("No native executed");
        return 0;
    }

    cell_t param = params[1];
    SPExt::NativeCallback *nativeCallback = SPExt::NativeCallback::currentPluginNative.top();
    std::int32_t paramsNum = nativeCallback->getParamAsInt(0);
    if (param > paramsNum || param < 0)
    {
        ctx->ReportError("Incorrect parameter %d (range: 0 - %d)", param, paramsNum);
        return 0;
    }

    cell_t *arrayToCopy;
    ctx->LocalToPhysAddr(params[2], &arrayToCopy);
    auto destArray = reinterpret_cast<float *>(nativeCallback->getParamAsArray(param));

    for (std::size_t i = 0; i < static_cast<std::size_t>(params[3]); i++)
    {
        destArray[i] = sp_ctof(arrayToCopy[i]);
    }

    return 1;
}

// void ChangeLevel(const char[] map)
static cell_t ChangeLevel(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_map = 1
    };

    char *newMap;
    ctx->LocalToString(params[arg_map], &newMap);
    gEngine->changeLevel(newMap, "", Metamod::FuncCallType::Hooks);

    return 1;
}

// float GetGameTime()
static cell_t GetGameTime(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params [[maybe_unused]])
{
    return sp_ftoc(gEngine->getTime());
}

// native void ServerCmd(const char[] command, any ...)
static cell_t ServerCmd(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_buffer = 1,
        arg_params
    };

    char *spFormatString;
    char bufferOutput[1024];

    ctx->LocalToString(params[arg_buffer], &spFormatString);
    std::size_t res = formatString(bufferOutput, sizeof(bufferOutput) - 2, spFormatString, ctx, params, arg_params);

    bufferOutput[res++] = '\n';
    bufferOutput[res] = '\0';

    gEngine->serverCommand(bufferOutput, Metamod::FuncCallType::Direct);

    return 1;
}

// native void ServerExec()
static cell_t ServerExec(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params [[maybe_unused]])
{
    gEngine->serverExecute(Metamod::FuncCallType::Direct);
    return 1;
}

// native int GetMapName(char[] mapname, int size)
static cell_t GetMapName(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_buffer = 1,
        arg_size
    };

    char *plOutput;
    ctx->LocalToString(params[arg_buffer], &plOutput);

    std::string_view mapName = gEngine->getMapName();

    return static_cast<cell_t>(gSPUtils->strCopy(plOutput, params[arg_size], mapName));
}

sp_nativeinfo_t gCoreNatives[] = {{"PrintToServer", PrintToServer},
                                  {"PrecacheModel", PrecacheModel},
                                  {"PrecacheSound", PrecacheSound},
                                  {"PrecacheGeneric", PrecacheGeneric},
                                  {"NativeRegister", NativeRegister},
                                  {"NativeGetInt", NativeGetInt},
                                  {"NativeGetIntRef", NativeGetIntRef},
                                  {"NativeGetFloat", NativeGetFloat},
                                  {"NativeGetFloatRef", NativeGetFloatRef},
                                  {"NativeGetString", NativeGetString},
                                  {"NativeGetArrayInt", NativeGetArrayInt},
                                  {"NativeGetArrayFloat", NativeGetArrayFloat},
                                  {"NativeSetIntRef", NativeSetIntRef},
                                  {"NativeSetFloatRef", NativeSetFloatRef},
                                  {"NativeSetString", NativeSetString},
                                  {"NativeSetArrayInt", NativeSetArrayInt},
                                  {"NativeSetArrayFloat", NativeSetArrayFloat},
                                  {"ChangeLevel", ChangeLevel},
                                  {"GetGameTime", GetGameTime},
                                  {"ServerCmd", ServerCmd},
                                  {"ServerExec", ServerExec},
                                  {"GetMapName", GetMapName},
                                  {nullptr, nullptr}};
