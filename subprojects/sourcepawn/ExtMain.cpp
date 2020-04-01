/*
 *  Copyright (C) 2018-2020 SPMod Development Team
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

using namespace SPExt;

SPMod::ISPGlobal *gSPGlobal;
std::unique_ptr<AdapterInterface> gAdapterInterface;
SPMod::ILogger *gSPLogger;

SPMod::ICommandMngr *gSPCmdMngr;
SPMod::ICvarMngr *gSPCvarMngr;
SPMod::IForwardMngr *gSPFwdMngr;
SPMod::ILoggerMngr *gSPLoggerMngr;
SPMod::IMenuMngr *gSPMenuMngr;
SPMod::IPlayerMngr *gSPPlrMngr;
SPMod::ITimerMngr *gSPTimerMngr;
SPMod::IUtils *gSPUtils;
SPMod::INativeProxy *gSPNativeProxy;
SPMod::IVTableHookManager *gSPVTHookMngr;
SPMod::Engine::IEngine *gSPEngine;
SPMod::Engine::IFuncs *gSPEngFuncs;
SPMod::Engine::IFuncs *gSPEngFuncsHooked;
SPMod::Engine::IGlobals *gSPEngGlobals;
SPMod::Metamod::IMetamod *gSPMetamod;
SPMod::Metamod::IFuncs *gSPMetamodFuncs;

TypeHandler<SPMod::Engine::ITraceResult> gTraceResultHandlers;

namespace
{
    bool checkInterfacesVersion()
    {
        gSPCmdMngr = gSPGlobal->getCommandManager();
        if (!gSPCmdMngr->isVersionCompatible(SPMod::ICommandMngr::VERSION))
        {
            return false;
        }

        gSPCvarMngr = gSPGlobal->getCvarManager();
        if (!gSPCvarMngr->isVersionCompatible(SPMod::ICvarMngr::VERSION))
        {
            return false;
        }

        gSPFwdMngr = gSPGlobal->getForwardManager();
        if (!gSPFwdMngr->isVersionCompatible(SPMod::IForwardMngr::VERSION))
        {
            return false;
        }

        gSPLoggerMngr = gSPGlobal->getLoggerManager();
        if (!gSPLoggerMngr->isVersionCompatible(SPMod::ILoggerMngr::VERSION))
        {
            return false;
        }

        gSPMenuMngr = gSPGlobal->getMenuManager();
        if (!gSPMenuMngr->isVersionCompatible(SPMod::IMenuMngr::VERSION))
        {
            return false;
        }

        gSPPlrMngr = gSPGlobal->getPlayerManager();
        if (!gSPPlrMngr->isVersionCompatible(SPMod::IPlayerMngr::VERSION))
        {
            return false;
        }

        gSPTimerMngr = gSPGlobal->getTimerManager();
        if (!gSPTimerMngr->isVersionCompatible(SPMod::ITimerMngr::VERSION))
        {
            return false;
        }

        gSPUtils = gSPGlobal->getUtils();
        if (!gSPUtils->isVersionCompatible(SPMod::IUtils::VERSION))
        {
            return false;
        }

        gSPNativeProxy = gSPGlobal->getNativeProxy();
        if (!gSPNativeProxy->isVersionCompatible(SPMod::INativeProxy::VERSION))
        {
            return false;
        }

        gSPVTHookMngr = gSPGlobal->getVTableManager();
        if (!gSPVTHookMngr->isVersionCompatible(SPMod::IVTableHookManager::VERSION))
        {
            return false;
        }

        gSPEngine = gSPGlobal->getEngine();
        if (!gSPEngine->isVersionCompatible(SPMod::Engine::IEngine::VERSION))
        {
            return false;
        }

        gSPEngFuncs = gSPEngine->getFuncs();
        gSPEngFuncsHooked = gSPEngine->getFuncs(true);
        gSPEngGlobals = gSPEngine->getGlobals();

        gSPMetamod = gSPGlobal->getMetamod();
        if (!gSPMetamod->isVersionCompatible(SPMod::Metamod::IMetamod::VERSION))
        {
            return false;
        }

        gSPMetamodFuncs = gSPMetamod->getFuncs();

        return true;
    }

    bool pushParamsToFunc(SPMod::IForward *fwd, SourcePawn::IPluginFunction *func, cell_t *result)
    {
        using variantArray = std::variant<int *, float *>;

        if (!func || !func->IsRunnable())
            return false;

        for (const auto param : fwd->getParams())
        {
            auto paramType = param->getDataType();

            if (paramType & SPMod::IForward::IParam::Type::Int || paramType & SPMod::IForward::IParam::Type::Float)
            {
                if (paramType & SPMod::IForward::IParam::Type::Array)
                {
                    if (paramType & SPMod::IForward::IParam::Type::Int)
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
                else if (paramType & SPMod::IForward::IParam::Type::Pointer)
                {
                    if (paramType & SPMod::IForward::IParam::Type::Int)
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
                    if (paramType & SPMod::IForward::IParam::Type::Int)
                    {
                        func->PushCell(std::any_cast<int>(param->getData()));
                    }
                    else
                    {
                        func->PushFloat(std::any_cast<float>(param->getData()));
                    }
                }
            }
            else if (paramType & SPMod::IForward::IParam::Type::String)
            {
                if (param->shouldCopyback() || param->getStringFlags() != SPMod::IForward::StringFlags::None)
                {
                    int spStringFlags = 0;
                    auto stringFlags = SPMod::IForward::StringFlags::None;

                    if (stringFlags & SPMod::IForward::StringFlags::Utf8)
                        spStringFlags |= SM_PARAM_STRING_UTF8;

                    if (stringFlags & SPMod::IForward::StringFlags::Copy)
                        spStringFlags |= SM_PARAM_STRING_COPY;

                    if (stringFlags & SPMod::IForward::StringFlags::Binary)
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

SPMOD_API SPMod::ExtQueryValue SPMod_Query(SPMod::ISPGlobal *spmodInstance)
{
    gSPGlobal = spmodInstance;
    gAdapterInterface = std::make_unique<AdapterInterface>(gSPGlobal->getPath(SPMod::DirType::Exts));

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
        Plugin *fwdPlugin = gAdapterInterface->getPluginMngr()->getPlugin(fwd->getPlugin());
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

            for (const auto &pair : gAdapterInterface->getPluginMngr()->getPluginsListCore())
            {
                Plugin *plugin = pair.second.get();
                SourcePawn::IPluginFunction *func = plugin->getRuntime()->GetFunctionByName(fwd->getName().data());

                if (!func || !func->IsRunnable())
                    continue;

                cell_t fwdResult = 0;
                bool succeed = pushParamsToFunc(fwd, func, &fwdResult);

                if (!succeed || execType & SPMod::IForward::ExecType::Ignore)
                    continue;

                if (result < fwdResult)
                    result = fwdResult;

                if (execType & SPMod::IForward::ExecType::Stop &&
                    fwdResult == static_cast<cell_t>(SPMod::IForward::ReturnValue::Stop))
                {
                    if (!(execType & SPMod::IForward::ExecType::Highest))
                        result = fwdResult;

                    stop = true;
                    break;
                }
            }
        }
    });

    gSPVTHookMngr->addCallback([](SPMod::IVTableHook *const hook, bool post) {
        std::pair<std::unordered_multimap<SPMod::IVTableHook *, SourcePawn::IPluginFunction *>::iterator,
            std::unordered_multimap<SPMod::IVTableHook *, SourcePawn::IPluginFunction *>::iterator>
            range;
        if (!post)
        {
            range = gVTableHook.equal_range(hook);
        }
        else
        {
            range = gVTableHookPost.equal_range(hook);
        }

        cell_t finalResult = 0;
        for (auto it = range.first; it != range.second; it++)
        {
            SourcePawn::IPluginFunction *func = it->second;
            if (func && func->IsRunnable())
            {
                cell_t result = 0;
                func->PushCell(gVTableHandlers.getKey(hook));
                std::vector<std::size_t> traceResultHandlersIds;
                for (const auto &param : hook->getParams())
                {
                    std::visit(
                        [=, &traceResultHandlersIds](auto &&arg) {
                          using T = std::decay_t<decltype(arg)>;

                          if constexpr (std::is_same_v<T, std::int32_t> || std::is_same_v<T, std::int16_t> ||
                                        std::is_same_v<T, std::uint32_t>)
                          {
                              func->PushCell(arg);
                          }
                          else if constexpr (std::is_same_v<T, float>)
                          {
                              func->PushFloat(arg);
                          }
                          else if constexpr (std::is_same_v<T, float *>)
                          {
                              func->PushArray(reinterpret_cast<cell_t *>(arg), 3);
                          }
                          else if constexpr (std::is_same_v<T, SPMod::Engine::IEntVars *> ||
                                             std::is_same_v<T, SPMod::Engine::IEdict *>)
                          {
                              func->PushCell(arg->getIndex());
                          }
                          else if constexpr (std::is_same_v<T, SPMod::Engine::ITraceResult *>)
                          {
                              func->PushCell(traceResultHandlersIds.emplace_back(gTraceResultHandlers.create(arg)));
                          }
                          else if constexpr (std::is_same_v<T, const char *>)
                          {
                              func->PushString(arg);
                          }
                        },
                        param);
                }

                func->Execute(&result);

                for (auto traceResultHandle : traceResultHandlersIds)
                {
                    gTraceResultHandlers.free(traceResultHandle);
                }

                if (result == SPMod::IVTableHook::Return::Supercede)
                {
                    return SPMod::IVTableHook::Return::Supercede;
                }
                else if (result > finalResult)
                {
                    finalResult = result;
                }
            }
        }
        return static_cast<SPMod::IVTableHook::Return>(finalResult);
    });
    return true;
}

SPMOD_API void SPMod_End() {}
