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

    return PRECACHE_MODEL(STRING(ALLOC_STRING(modelToPrecache)));
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

    return PRECACHE_SOUND(STRING(ALLOC_STRING(soundToPrecache)));
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

    return PRECACHE_GENERIC(STRING(ALLOC_STRING(genericToPrecache)));
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

// native bool createForward(const char[] name, exectype, ...)
static cell_t core_createForward(SourcePawn::IPluginContext *ctx,
                                    const cell_t *params)
{
    char *pluginId, *fwdName;
    auto execType = static_cast<IForward::ExecType>(params[2]);
    auto &fwdManager = gSPGlobal->getForwardManagerCore();

    ctx->LocalToString(params[1], &fwdName);
    ctx->GetKey(1, reinterpret_cast<void **>(&pluginId));
    auto plOwner = gSPGlobal->getPluginManagerCore()->getPluginCore(pluginId);

    size_t fwdParamsNum = params[0] - 2;
    fwdParamTypeList fwdParamsList;

    for (size_t i = 0; i < fwdParamsNum; ++i)
    {
        cell_t *paramType;
        ctx->LocalToPhysAddr(params[i + 3], &paramType);
        fwdParamsList.at(i) = static_cast<IForward::ParamType>(*paramType);
    }

    auto plForward = std::make_shared<Forward>(fwdName,
                                                plOwner,
                                                fwdParamsList,
                                                fwdParamsNum,
                                                execType);

    if (!plForward)
    {
        ctx->ReportErrorNumber(SP_ERROR_OUT_OF_MEMORY);
        return 0;
    }

    return fwdManager->addForward(plForward);
}

struct FloatFromNative
{
    FloatFromNative(float val, size_t param) : m_realVal(val), m_param(param) { }
    float m_realVal;
    size_t m_param;
};

// native bool executeForward(const char[] name, int &result = 0, ...)
static cell_t core_executeForward(SourcePawn::IPluginContext *ctx,
                                    const cell_t *params)
{
    using ptype = IForward::ParamType;
    char *fwdName;
    cell_t *retResult;
    const std::unique_ptr<ForwardMngr> &fwdManager = gSPGlobal->getForwardManagerCore();

    ctx->LocalToString(params[1], &fwdName);
    ctx->LocalToPhysAddr(params[2], &retResult);

    std::shared_ptr<Forward> fwdToExecute = fwdManager->findForwardCore(fwdName);    
    if (!fwdToExecute)
    {
        fwdManager->resetPreparedParams();
        return 0;
    }
    
    size_t fwdParamsPassed = params[0] - 2;
    if (fwdParamsPassed < fwdToExecute->getParamsNum())
    {
        fwdManager->resetPreparedParams();
        return 0;
    }

    size_t floatValues = 0;
    std::array<std::shared_ptr<FloatFromNative>, SP_MAX_EXEC_PARAMS> floatsToCopyBack;

    size_t fwdParamsNum = fwdToExecute->getParamsNum();
    cell_t *paramToPass;
    for (size_t i = 3; i < fwdParamsNum + 3; ++i)
    {
        ptype paramType = fwdToExecute->getParamType(i);
        if (paramType == ptype::Cell)
        {
            ctx->LocalToPhysAddr(params[i], &paramToPass);
            fwdToExecute->pushCell(*paramToPass);
        }
        else if (paramType == ptype::CellRef)
        {
            ctx->LocalToPhysAddr(params[i], &paramToPass);
            fwdToExecute->pushCellPtr(paramToPass, true);
        }
        else if (paramType == ptype::Float)
        {
            ctx->LocalToPhysAddr(params[i], &paramToPass);
            fwdToExecute->pushFloat(sp_ctof(*paramToPass));
        }
        else if (paramType == ptype::FloatRef)
        {
            ctx->LocalToPhysAddr(params[i], &paramToPass);
            auto floatStr = std::make_shared<FloatFromNative>(sp_ctof(*paramToPass), i);

            fwdToExecute->pushFloatPtr(&floatStr->m_realVal, true);

            floatsToCopyBack.at(floatValues++) = floatStr;
        }
        else if (paramType == ptype::Array)
        {
            ctx->LocalToPhysAddr(params[i], &paramToPass);
            auto param = fwdManager->getParam<cell_t *>(*paramToPass);
            if (!param.has_value())
            {
                fwdManager->resetPreparedParams();
                fwdToExecute->resetParams();
                ctx->ReportError("Passed argument is not prepared array! %d", i);
                return 0;
            }

            fwdToExecute->pushArray(*param, fwdManager->getParamSize(*paramToPass),
                                            fwdManager->getParamCb(*paramToPass));

        }
        else if (paramType == ptype::String)
        {
            char *strToPass;
            ctx->LocalToString(params[i], &strToPass);
            fwdToExecute->pushString(strToPass);
        }
        else if (paramType == ptype::StringEx)
        {
            ctx->LocalToPhysAddr(params[i], &paramToPass);
            auto param = fwdManager->getParam<char *>(*paramToPass);
            if (!param.has_value())
            {
                fwdManager->resetPreparedParams();
                fwdToExecute->resetParams();
                ctx->ReportError("Passed argument is not prepared string! %d", i);
                return 0;
            }

            fwdToExecute->pushStringEx(*param,
                                        fwdManager->getParamSize(*paramToPass),
                                        fwdManager->getParamSf(*paramToPass),
                                        fwdManager->getParamCb(*paramToPass));
        }
    }

    fwdManager->resetPreparedParams();
    bool result = fwdToExecute->execFunc(retResult);

    // Copyback float values
    for (size_t i = 0; i < floatValues; ++i)
    {
        std::shared_ptr<FloatFromNative> floatStr = floatsToCopyBack.at(i);
        ctx->LocalToPhysAddr(params[floatStr->m_param], &paramToPass);
        *paramToPass = sp_ftoc(floatStr->m_realVal);
    }

    return result;
}

// native int prepareFwdArray(int[] array, int size, bool copyback)
static cell_t core_prepareFwdArray(SourcePawn::IPluginContext *ctx,
                                        const cell_t *params)
{
    cell_t *arrayForFwd;
    auto &fwdManager = gSPGlobal->getForwardManagerCore();

    ctx->LocalToPhysAddr(params[1], &arrayForFwd);
    auto id = fwdManager->addParam(arrayForFwd, params[2], params[3], IForward::StringFlags::None);

    return id.has_value() ? *id : -1;
}

// native int prepareFwdString(char[] array, int size, bool copyback, int stringflags)
static cell_t core_prepareFwdString(SourcePawn::IPluginContext *ctx,
                                        const cell_t *params)
{
    char *arrayForFwd;
    auto &fwdManager = gSPGlobal->getForwardManagerCore();

    ctx->LocalToString(params[1], &arrayForFwd);
    auto id = fwdManager->addParam(arrayForFwd, params[2], params[3], static_cast<IForward::StringFlags>(params[4]));

    return id.has_value() ? *id : -1;
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
    { "executeForward",     core_executeForward     },
    { "prepareFwdArray",    core_prepareFwdArray    },
    { "prepareFwdString",   core_prepareFwdString   },
    { nullptr,              nullptr                 }
};
