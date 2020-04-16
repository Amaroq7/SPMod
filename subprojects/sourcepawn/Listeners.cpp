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

namespace
{
    bool pushParamsToFunc(SPMod::IForward *fwd, SourcePawn::IPluginFunction *func, cell_t *result)
    {
        using variantArray = std::variant<int *, float *>;

        if (!func || !func->IsRunnable())
            return false;

        for (const auto param : fwd->getParams())
        {
            auto paramType = param->getDataType();

            if (hasEnumFlag(paramType, SPMod::IForward::IParam::Type::Int) ||
                hasEnumFlag(paramType, SPMod::IForward::IParam::Type::Float))
            {
                if (hasEnumFlag(paramType, SPMod::IForward::IParam::Type::Array))
                {
                    if (hasEnumFlag(paramType, SPMod::IForward::IParam::Type::Int))
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
                else if (hasEnumFlag(paramType, SPMod::IForward::IParam::Type::Pointer))
                {
                    if (hasEnumFlag(paramType, SPMod::IForward::IParam::Type::Int))
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
                    if (hasEnumFlag(paramType, SPMod::IForward::IParam::Type::Int))
                    {
                        func->PushCell(std::any_cast<int>(param->getData()));
                    }
                    else
                    {
                        func->PushFloat(std::any_cast<float>(param->getData()));
                    }
                }
            }
            else if (hasEnumFlag(paramType, SPMod::IForward::IParam::Type::String))
            {
                if (param->shouldCopyback() || param->getStringFlags() != SPMod::IForward::StringFlags::None)
                {
                    int spStringFlags = 0;
                    auto stringFlags = SPMod::IForward::StringFlags::None;

                    if (hasEnumFlag(stringFlags, SPMod::IForward::StringFlags::Utf8))
                        spStringFlags |= SM_PARAM_STRING_UTF8;

                    if (hasEnumFlag(stringFlags, SPMod::IForward::StringFlags::Copy))
                        spStringFlags |= SM_PARAM_STRING_COPY;

                    if (hasEnumFlag(stringFlags, SPMod::IForward::StringFlags::Binary))
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

namespace SPExt::Listener
{
    void Forward(SPMod::IForward *const fwd, int &result, bool &stop)
    {
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

                if (!succeed || hasEnumFlag(execType, SPMod::IForward::ExecType::Ignore))
                    continue;

                if (result < fwdResult)
                    result = fwdResult;

                if (hasEnumFlag(execType, SPMod::IForward::ExecType::Stop) &&
                    fwdResult == static_cast<cell_t>(SPMod::IForward::ReturnValue::Stop))
                {
                    if (!hasEnumFlag(execType, SPMod::IForward::ExecType::Highest))
                        result = fwdResult;

                    stop = true;
                    break;
                }
            }
        }
    }

    void Cvar(const SPMod::ICvar *const cvar, std::string_view old_value, std::string_view new_value)
    {
        auto range = gCvarPluginsCallbacks.equal_range(const_cast<SPMod::ICvar *>(cvar));
        for (auto it = range.first; it != range.second; it++)
        {
            SourcePawn::IPluginFunction *func = it->second;

            if (!func || !func->IsRunnable())
                continue;

            func->PushCell(gCvarsHandlers.getKey(const_cast<SPMod::ICvar *>(cvar)));
            func->PushString(old_value.data());
            func->PushString(new_value.data());
            func->Execute(nullptr);
        }
    }

    void Menu(SPMod::IMenu *const menu, SPMod::IMenu::IItem *const item, SPMod::IPlayer *const player, std::any data)
    {
        auto func = std::any_cast<SourcePawn::IPluginFunction *>(data);
        if (func && func->IsRunnable())
        {
            std::size_t menuId = gMenuHandlers.getKey(menu);
            func->PushCell(menuId);

            if (item->getNavType() == SPMod::NavigationType::None)
            {
                func->PushCell(
                    static_cast<cell_t>(menuPackItem(menuId, static_cast<cell_t>(menu->getItemIndex(item)))));
            }
            else
            {
                func->PushCell(static_cast<cell_t>(item->getNavType()));
            }

            func->PushCell(static_cast<cell_t>(player->getIndex()));
            func->Execute(nullptr);
        }
    }

    void MenuText(SPMod::IMenu *const menu, int key, SPMod::IPlayer *const player, std::any data)
    {
        auto func = std::any_cast<SourcePawn::IPluginFunction *>(data);
        if (func && func->IsRunnable())
        {
            func->PushCell(static_cast<cell_t>(gMenuHandlers.getKey(menu)));
            func->PushCell(static_cast<cell_t>(key));
            func->PushCell(static_cast<cell_t>(player->getIndex()));
            func->Execute(nullptr);
        }
    }

    SPMod::IMenu::IItem::Status MenuItemCallback(SPMod::IMenu *const menu,
                                                 SPMod::IMenu::IItem *const item,
                                                 SPMod::IPlayer *const player,
                                                 std::any data)
    {
        auto func = std::any_cast<SourcePawn::IPluginFunction *>(data);
        if (func && func->IsRunnable())
        {
            cell_t menuId = gMenuHandlers.getKey(menu);
            cell_t result;
            func->PushCell(menuId);
            // item index?
            func->PushCell(static_cast<cell_t>(menuPackItem(menuId, menu->getItemIndex(item))));
            func->PushCell(static_cast<cell_t>(player->getIndex()));
            func->Execute(reinterpret_cast<cell_t *>(&result));

            return static_cast<SPMod::IMenu::IItem::Status>(result);
        }

        return SPMod::IMenu::IItem::Status::Enabled;
    }

    SPMod::IForward::ReturnValue CmdCallback(SPMod::IPlayer *const player, SPMod::ICommand *const cmd, std::any data)
    {
        auto spFunc = std::any_cast<SourcePawn::IPluginFunction *>(data);
        if (spFunc)
        {
            cell_t retValue;
            spFunc->PushCell(static_cast<cell_t>(player->getIndex()));
            spFunc->PushCell(static_cast<cell_t>(gCommandHandlers.getKey(cmd)));
            spFunc->Execute(&retValue);

            return static_cast<SPMod::IForward::ReturnValue>(retValue);
        }

        return SPMod::IForward::ReturnValue::Ignored;
    }

    bool TimerCallback(SPMod::ITimer *const timer, std::any data)
    {
        auto func = std::any_cast<SourcePawn::IPluginFunction *>(data);
        if (func && func->IsRunnable())
        {
            cell_t result = 0;
            func->PushCell(gTimerHandlers.getKey(timer));
            func->PushCell(std::any_cast<cell_t>(timer->getData()));
            func->Execute(&result);

            return static_cast<SPMod::IForward::ReturnValue>(result) != SPMod::IForward::ReturnValue::Stop;
        }

        return true;
    }

    SPMod::IForward::ReturnValue MessageHookCallback(SPMod::IMessage *const message, std::any cbData)
    {
        auto func = std::any_cast<SourcePawn::IPluginFunction *>(cbData);
        if (func && func->IsRunnable())
        {
            // typedef MessageHandler = function PluginReturn (MessageDest dest, int type, int receiver);
            cell_t retValue = 0;
            func->PushCell(message->getDest());
            func->PushCell(message->getType());
            func->PushCell(message->getEdict()->getIndex());
            func->Execute(&retValue);

            return static_cast<SPMod::IForward::ReturnValue>(retValue);
        }
        return SPMod::IForward::ReturnValue::Ignored;
    }

} // namespace SPExt::Listener
