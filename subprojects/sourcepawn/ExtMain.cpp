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

#include "ExtMain.hpp"

#include "SourcePawnAPI.hpp"
#include "AdapterInterface.hpp"
#include "CmdNatives.hpp"
#include "TimerNatives.hpp"
#include "StringNatives.hpp"
#include "CoreNatives.hpp"
#include "PlayerNatives.hpp"
#include "ForwardNatives.hpp"
#include "MenuNatives.hpp"
#include "MessageNatives.hpp"
#include "PluginSystem.hpp"
#include "DebugListener.hpp"
#include "HooksNatives.hpp"

#include <ISPGlobal.hpp>
#include <metamodcpp_sdk/IMetamod.hpp>
#include <metamodcpp_sdk/engine/ITraceResult.hpp>
#include <metamodcpp_sdk/IHelpers.hpp>

using namespace SPExt;

SPMod::ISPGlobal *gSPGlobal;
SPMod::ILogger *gSPLogger;
SPMod::ILoggerMngr *gSPLoggerMngr;
Metamod::IMetamod *gMetaAPI;
Metamod::Engine::ILibrary *gEngine;
Metamod::Game::ILibrary *gGame;

namespace
{
    bool checkInterfacesVersion()
    {
        bool cmdNativesInit = SPExt::initCmdNatives();
        bool timerNativesInit = SPExt::initTimerNatives();
        bool stringNativesInit = SPExt::initStringNatives();
        bool coreNativesInit = SPExt::initCoreNatives();
        bool playerNativesInit = SPExt::initPlayerNatives();
        bool forwardNativesInit = SPExt::initForwardNatives();
        bool menuNativesInit = SPExt::initMenuNatives();
        bool msgNativesInit = SPExt::initMsgNatives();

        gSPLoggerMngr = gSPGlobal->getLoggerManager();
        if (!gSPLoggerMngr->isVersionCompatible(SPMod::ILoggerMngr::VERSION))
        {
            return false;
        }

        return cmdNativesInit && timerNativesInit && stringNativesInit && coreNativesInit
               && playerNativesInit && forwardNativesInit && menuNativesInit && msgNativesInit;
    }

    bool pushParamsToFunc(SPMod::IForward *fwd, SourcePawn::IPluginFunction *func, cell_t *result)
    {
        using variantArray = std::variant<int *, float *>;

        if (!func || !func->IsRunnable())
            return false;

        for (const auto param : fwd->getParams())
        {
            auto paramType = param->getDataType();

            if ((paramType & SPMod::IForward::IParam::Type::Int) == SPMod::IForward::IParam::Type::Int ||
                (paramType & SPMod::IForward::IParam::Type::Float) == SPMod::IForward::IParam::Type::Float)
            {
                if ((paramType & SPMod::IForward::IParam::Type::Array) == SPMod::IForward::IParam::Type::Array)
                {
                    if ((paramType & SPMod::IForward::IParam::Type::Int) == SPMod::IForward::IParam::Type::Int)
                    {
                        int *paramData = std::get<int *>(std::any_cast<variantArray>(param->getData()));
                        func->PushArray(paramData, param->getDataSize(),
                                        (param->shouldCopyback() ? SM_PARAM_COPYBACK : 0));
                    }
                    else
                    {
                        float *paramData = std::get<float *>(std::any_cast<variantArray>(param->getData()));
                        func->PushArray(reinterpret_cast<cell_t *>(paramData), param->getDataSize(),
                                        (param->shouldCopyback() ? SM_PARAM_COPYBACK : 0));
                    }
                }
                else if ((paramType & SPMod::IForward::IParam::Type::Pointer) == SPMod::IForward::IParam::Type::Pointer)
                {
                    if ((paramType & SPMod::IForward::IParam::Type::Int) == SPMod::IForward::IParam::Type::Int)
                    {
                        func->PushCellByRef(std::any_cast<int *>(param->getData()),
                                            (param->shouldCopyback() ? SM_PARAM_COPYBACK : 0));
                    }
                    else
                    {
                        func->PushFloatByRef(std::any_cast<float *>(param->getData()),
                                             (param->shouldCopyback() ? SM_PARAM_COPYBACK : 0));
                    }
                }
                else
                {
                    if ((paramType & SPMod::IForward::IParam::Type::Int) == SPMod::IForward::IParam::Type::Int)
                    {
                        func->PushCell(std::any_cast<int>(param->getData()));
                    }
                    else
                    {
                        func->PushFloat(std::any_cast<float>(param->getData()));
                    }
                }
            }
            else if ((paramType & SPMod::IForward::IParam::Type::String) == SPMod::IForward::IParam::Type::String)
            {
                if (param->shouldCopyback() || param->getStringFlags() != SPMod::IForward::StringFlags::None)
                {
                    int spStringFlags = 0;
                    auto stringFlags = SPMod::IForward::StringFlags::None;

                    if ((stringFlags & SPMod::IForward::StringFlags::Utf8) == SPMod::IForward::StringFlags::Utf8)
                        spStringFlags |= SM_PARAM_STRING_UTF8;

                    if ((stringFlags & SPMod::IForward::StringFlags::Copy) == SPMod::IForward::StringFlags::Copy)
                        spStringFlags |= SM_PARAM_STRING_COPY;

                    if ((stringFlags & SPMod::IForward::StringFlags::Binary) == SPMod::IForward::StringFlags::Binary)
                        spStringFlags |= SM_PARAM_STRING_BINARY;

                    func->PushStringEx(std::any_cast<char *>(param->getData()), param->getDataSize(),
                                       (param->shouldCopyback() ? SM_PARAM_COPYBACK : 0), spStringFlags);
                }
                else
                {
                    func->PushString(std::any_cast<std::string>(param->getData()).c_str());
                }
            }
        }

        return func->Execute(result) == SP_ERROR_NONE;
    }
} // namespace

SPMOD_API SPMod::ExtQueryValue SPMod_Query(SPMod::ISPGlobal *spmodInstance, Metamod::IMetamod *metaAPI)
{
    gSPGlobal = spmodInstance;
    gAdapterInterface = std::make_unique<AdapterInterface>(gSPGlobal->getPath(SPMod::DirType::Exts));
    gMetaAPI = metaAPI;
    gEngine = metaAPI->getEngine();
    gGame = metaAPI->getGame();

    if (!checkInterfacesVersion())
    {
        return SPMod::ExtQueryValue::DontLoad;
    }

    return (gSPGlobal->registerAdapter(gAdapterInterface.get()) ? SPMod::ExtQueryValue::SPModAdapter
                                                                : SPMod::ExtQueryValue::DontLoad);
}

SPMOD_API bool SPMod_Init()
{
    gSPLogger = gSPLoggerMngr->getLogger(gSPExtLoggerName);
    gSPLogger->setLogLevel(SPMod::LogLevel::Info);

    gSPAPI = std::make_unique<SourcePawnAPI>(gSPGlobal->getPath(SPMod::DirType::Exts));

    gSPFwdMngr->addForwardListener([](SPMod::IForward *const fwd, int &result, bool &stop) {
        // One plugin forward
        Plugin *fwdPlugin = gAdapterInterface->getPluginMngrImpl()->getPlugin(fwd->getPlugin());
        if (fwdPlugin)
        {
            SourcePawn::IPluginFunction *func = fwdPlugin->getRuntime()->GetFunctionByName(fwd->getName().data());

            if (!func || !func->IsRunnable())
                return;

            cell_t fwdResult = 0;
            if (!pushParamsToFunc(fwd, func, &fwdResult))
            {
                return;
            }

            result = fwdResult;
        }
        else
        {
            SPMod::IForward::ExecType execType = fwd->getExecType();

            for (const auto &pair : gAdapterInterface->getPluginMngrImpl()->getPluginsListImpl())
            {
                Plugin *plugin = pair.second.get();
                SourcePawn::IPluginFunction *func = plugin->getRuntime()->GetFunctionByName(fwd->getName().data());

                if (!func || !func->IsRunnable())
                    continue;

                cell_t fwdResult = 0;
                bool succeed = pushParamsToFunc(fwd, func, &fwdResult);

                if (!succeed ||
                    (execType & SPMod::IForward::ExecType::Ignore) == SPMod::IForward::ExecType::Ignore)
                    continue;

                if (result < fwdResult)
                    result = fwdResult;

                if ((execType & SPMod::IForward::ExecType::Stop) == SPMod::IForward::ExecType::Stop &&
                    fwdResult == static_cast<cell_t>(SPMod::IForward::ReturnValue::Stop))
                {
                    if ((execType & SPMod::IForward::ExecType::Highest) != SPMod::IForward::ExecType::Highest)
                        result = fwdResult;

                    stop = true;
                    break;
                }
            }
        }
    });

    return true;
}

SPMOD_API void SPMod_End()
{
    uninstallHooks();
}
