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

// native void printToConsole(const char[] text, ...)
static cell_t core_printToConsole(SourcePawn::IPluginContext *ctx,
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
static cell_t core_precacheModel(SourcePawn::IPluginContext *ctx,
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
static cell_t core_precacheSound(SourcePawn::IPluginContext *ctx,
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
static cell_t core_precacheGeneric(SourcePawn::IPluginContext *ctx,
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

// native int numToString(int num, char[] converted_num, int size)
static cell_t core_numToString(SourcePawn::IPluginContext *ctx,
                                    const cell_t *params)
{
    auto numToConvert = params[1];
    auto numConverted = std::to_string(numToConvert);
    ctx->StringToLocal(params[2], params[3], numConverted.c_str());

    return numConverted.length();
}

// native int realToString(float real, char[] converted_real, int size)
static cell_t core_realToString(SourcePawn::IPluginContext *ctx,
                                    const cell_t *params)
{
    auto realToConvert = sp_ctof(params[1]);
    auto realConverted = std::to_string(realToConvert);
    ctx->StringToLocal(params[2], params[3], realConverted.c_str());

    return realConverted.length();
}

static cell_t core_copyString(SourcePawn::IPluginContext *ctx,
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

// native int createForward(const char[] name, ExecType exectype, ForwardParams ...)
static cell_t core_createForward(SourcePawn::IPluginContext *ctx,
                                 const cell_t *params)
{
    char *fwdName;
    auto execType = static_cast<IForward::ExecType>(params[2]);
    const std::unique_ptr<ForwardMngr> &fwdManager = gSPGlobal->getForwardManagerCore();

    ctx->LocalToString(params[1], &fwdName);

    size_t fwdParamsNum = params[0] - 2;

    if (fwdParamsNum >= SP_MAX_EXEC_PARAMS)
        return -1;

    std::array<IForward::ParamType, SP_MAX_EXEC_PARAMS> fwdParamsList;

    for (size_t i = 0; i < fwdParamsNum; ++i)
    {
        cell_t *paramType;
        ctx->LocalToPhysAddr(params[i + 3], &paramType);
        fwdParamsList.at(i) = static_cast<IForward::ParamType>(*paramType);
    }

    std::shared_ptr<Forward> plForward = fwdManager->createForwardCore(fwdName, execType, fwdParamsList, fwdParamsNum);

    if (!plForward)
        return 0;

    return plForward->getId();
}

// native bool core_pushStart(int forward_id)
static cell_t core_pushStart(SourcePawn::IPluginContext *ctx,
                             const cell_t *params)
{
    if (ForwardMngr::m_currentForward)
    {
        ctx->ReportError("Cannot start another call!");
        return 0;
    }

    std::shared_ptr<Forward> forward = gSPGlobal->getForwardManagerCore()->findForwardCore(params[1]);

    if (!forward)
        return 0;

    ForwardMngr::m_currentForward = forward;
    return 1;
}

// native bool pushCell(any value)
static cell_t core_pushCell(SourcePawn::IPluginContext *ctx,
                            const cell_t *params)
{
    if (!ForwardMngr::m_currentForward)
    {
        ctx->ReportError("Function calling was not started!");
        return 0;
    }

    return ForwardMngr::m_currentForward->pushCell(params[1]);
}

// native bool pushCellRef(any &cell)
static cell_t core_pushCellRef(SourcePawn::IPluginContext *ctx,
                               const cell_t *params)
{
    if (!ForwardMngr::m_currentForward)
    {
        ctx->ReportError("Function calling was not started!");
        return 0;
    }

    cell_t *value;
    ctx->LocalToPhysAddr(params[1], &value);
    return ForwardMngr::m_currentForward->pushCellPtr(value, true);
}

// native bool pushFloat(float real)
static cell_t core_pushFloat(SourcePawn::IPluginContext *ctx,
                             const cell_t *params)
{
    if (!ForwardMngr::m_currentForward)
    {
        ctx->ReportError("Function calling was not started!");
        return 0;
    }

    return ForwardMngr::m_currentForward->pushFloat(sp_ctof(params[1]));
}

// native bool pushFloatRef(float &real)
static cell_t core_pushFloatRef(SourcePawn::IPluginContext *ctx,
                                const cell_t *params)
{
    if (!ForwardMngr::m_currentForward)
    {
        ctx->ReportError("Function calling was not started!");
        return 0;
    }

    cell_t *real;
    ctx->LocalToPhysAddr(params[1], &real);
    return ForwardMngr::m_currentForward->pushFloatPtr(*reinterpret_cast<float **>(&real), true);
}

// native bool pushString(const char[] string)
static cell_t core_pushString(SourcePawn::IPluginContext *ctx,
                              const cell_t *params)
{
    if (!ForwardMngr::m_currentForward)
    {
        ctx->ReportError("Function calling was not started!");
        return 0;
    }

    char *string;
    ctx->LocalToString(params[1], &string);
    return ForwardMngr::m_currentForward->pushString(string);
}

// native bool pushArray(const any[] array, int size)
static cell_t core_pushArray(SourcePawn::IPluginContext *ctx,
                             const cell_t *params)
{
    if (!ForwardMngr::m_currentForward)
    {
        ctx->ReportError("Function calling was not started!");
        return 0;
    }

    cell_t *array;
    ctx->LocalToPhysAddr(params[1], &array);
    return ForwardMngr::m_currentForward->pushArray(array, params[2], false);
}

// native bool pushStringEx(char[] string, int size, int sflags, int cpflags)
static cell_t core_pushStringEx(SourcePawn::IPluginContext *ctx,
                                const cell_t *params)
{
    if (!ForwardMngr::m_currentForward)
    {
        ctx->ReportError("Function calling was not started!");
        return 0;
    }

    char *string;
    ctx->LocalToString(params[1], &string);
    auto sflags = static_cast<IForward::StringFlags>(params[3]);
    return ForwardMngr::m_currentForward->pushStringEx(string, params[2], sflags, params[4]);
}

// native bool pushArrayEx(any[] array, int size, int cpflags)
static cell_t core_pushArrayEx(SourcePawn::IPluginContext *ctx,
                               const cell_t *params)
{
    if (!ForwardMngr::m_currentForward)
    {
        ctx->ReportError("Function calling was not started!");
        return 0;
    }

    cell_t *array;
    ctx->LocalToPhysAddr(params[1], &array);
    return ForwardMngr::m_currentForward->pushArray(array, params[2], params[3]);
}

// native bool pushFinish(any &result = 0)
static cell_t core_pushFinish(SourcePawn::IPluginContext *ctx,
                              const cell_t *params)
{
    if (!ForwardMngr::m_currentForward)
    {
        ctx->ReportError("Function calling was not started!");
        return 0;
    }

    cell_t *retResult;
    ctx->LocalToPhysAddr(params[1], &retResult);

    bool result = ForwardMngr::m_currentForward->execFunc(retResult);

    // Only set to null if call succeed
    if (result)
        ForwardMngr::m_currentForward = nullptr;

    return result;
}

// native void pushCancel()
static cell_t core_pushCancel(SourcePawn::IPluginContext *ctx,
                              const cell_t *params)
{
    if (!ForwardMngr::m_currentForward)
    {
        ctx->ReportError("Function calling was not started!");
        return 0;
    }

    ForwardMngr::m_currentForward->resetParams();
    ForwardMngr::m_currentForward = nullptr;

    return 1;
}

// native bool nativeRegister(const char[] name, PluginNative func)
static cell_t core_nativeRegister(SourcePawn::IPluginContext *ctx,
                                  const cell_t *params)
{
    char *nativeName, *pluginIdentity;
    ctx->LocalToString(params[1], &nativeName);
    ctx->GetKey(1, reinterpret_cast<void **>(&pluginIdentity));

    SourcePawn::IPluginFunction *fnToExecute = ctx->GetFunctionById(params[2]);

    return gSPGlobal->getNativeManagerCore()->addFakeNative(pluginIdentity, nativeName, fnToExecute);
}

// native any nativeGetCell(int param)
static cell_t core_nativeGetCell(SourcePawn::IPluginContext *ctx,
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
static cell_t core_nativeGetCellRef(SourcePawn::IPluginContext *ctx,
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
static cell_t core_nativeGetString(SourcePawn::IPluginContext *ctx,
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

// native int nativeGetArray(int param, any[] buffer, int size)
static cell_t core_nativeGetArray(SourcePawn::IPluginContext *ctx,
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

// native int nativeSetCellRef(int param, any value)
static cell_t core_nativeSetCellRef(SourcePawn::IPluginContext *ctx,
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
static cell_t core_nativeSetString(SourcePawn::IPluginContext *ctx,
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

// native int nativeSetArray(int param, const any[] buffer, int size)
static cell_t core_nativeSetArray(SourcePawn::IPluginContext *ctx,
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
    { "printToConsole",     core_printToConsole     },
    { "precacheModel",      core_precacheModel      },
    { "precacheSound",      core_precacheSound      },
    { "precacheGeneric",    core_precacheGeneric    },
    { "numToString",        core_numToString        },
    { "realToString",       core_realToString       },
    { "copyString",         core_copyString         },
    { "createForward",      core_createForward      },
    { "pushStart",          core_pushStart          },
    { "pushCell",           core_pushCell           },
    { "pushCellRef",        core_pushCellRef        },
    { "pushFloat",          core_pushFloat          },
    { "pushFloatRef",       core_pushFloatRef       },
    { "pushString",         core_pushString         },
    { "pushArray",          core_pushArray          },
    { "pushStringEx",       core_pushStringEx       },
    { "pushArrayEx",        core_pushArrayEx        },
    { "pushFinish",         core_pushFinish         },
    { "pushCancel",         core_pushCancel         },
    { "nativeRegister",     core_nativeRegister     },
    { "nativeGetCell",      core_nativeGetCell      },
    { "nativeGetCellRef",   core_nativeGetCellRef   },
    { "nativeGetString",    core_nativeGetString    },
    { "nativeGetArray",     core_nativeGetArray     },
    { "nativeSetCellRef",   core_nativeSetCellRef   },
    { "nativeSetString",    core_nativeSetString    },
    { "nativeSetArray",     core_nativeSetArray     },
    { nullptr,              nullptr                 }
};
