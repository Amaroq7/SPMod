/*
 *  Copyright (C) 2018-2019 SPMod Development Team
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
    bool pushParamsToFunc(const SPMod::IForward *const fwd, SourcePawn::IPluginFunction *func, cell_t *result)
    {
        if (!func || !func->IsRunnable())
            return false;

        for (std::size_t i = 0; i < fwd->getParamsNum(); i++)
        {
            const SPMod::IForward::Param *param = fwd->getParam(i);
            SPMod::IForward::Param::Type paramType = param->m_dataType;

            if (hasEnumFlag(paramType, SPMod::IForward::Param::Type::Int) ||
                hasEnumFlag(paramType, SPMod::IForward::Param::Type::Float))
            {
                if (hasEnumFlag(paramType, SPMod::IForward::Param::Type::Array))
                    func->PushArray(reinterpret_cast<cell_t *>(param->m_data), param->m_size,
                                    (param->m_copyback ? SM_PARAM_COPYBACK : 0));

                else if (hasEnumFlag(paramType, SPMod::IForward::Param::Type::Pointer))
                {
                    if (hasEnumFlag(paramType, SPMod::IForward::Param::Type::Int))
                        func->PushCellByRef(reinterpret_cast<cell_t *>(param->m_data),
                                            (param->m_copyback ? SM_PARAM_COPYBACK : 0));
                    else
                        func->PushFloatByRef(reinterpret_cast<float *>(param->m_data),
                                             (param->m_copyback ? SM_PARAM_COPYBACK : 0));
                }
                else
                {
                    if (hasEnumFlag(paramType, SPMod::IForward::Param::Type::Int))
                        func->PushCell(*reinterpret_cast<cell_t *>(param->m_data));
                    else
                        func->PushFloat(*reinterpret_cast<float *>(param->m_data));
                }
            }
            else if (hasEnumFlag(paramType, SPMod::IForward::Param::Type::String))
            {
                if (param->m_copyback || param->m_stringFlags != SPMod::IForward::StringFlags::None)
                {
                    int spStringFlags = 0;
                    SPMod::IForward::StringFlags stringFlags = SPMod::IForward::StringFlags::None;

                    if (hasEnumFlag(stringFlags, SPMod::IForward::StringFlags::Utf8))
                        spStringFlags |= SM_PARAM_STRING_UTF8;

                    if (hasEnumFlag(stringFlags, SPMod::IForward::StringFlags::Copy))
                        spStringFlags |= SM_PARAM_STRING_COPY;

                    if (hasEnumFlag(stringFlags, SPMod::IForward::StringFlags::Binary))
                        spStringFlags |= SM_PARAM_STRING_BINARY;

                    func->PushStringEx(reinterpret_cast<char *>(param->m_data), param->m_size,
                                       (param->m_copyback ? SM_PARAM_COPYBACK : 0), spStringFlags);
                }
                else
                {
                    func->PushString(reinterpret_cast<const char *>(param->m_data));
                }
            }
        }

        return func->Execute(result) == SP_ERROR_NONE;
    }
} // namespace

namespace SPExt::Listener
{
    void Forward(const SPMod::IForward *const fwd, int *result, bool *stop)
    {
        // One plugin forward
        std::shared_ptr<Plugin> fwdPlugin = gModuleInterface->getPluginMngrCore()->getPluginCore(fwd->getPlugin());
        if (fwdPlugin)
        {
            SourcePawn::IPluginFunction *func = fwdPlugin->getRuntime()->GetFunctionByName(fwd->getName());

            if (!func || !func->IsRunnable())
                return;

            cell_t fwdResult = 0;
            pushParamsToFunc(fwd, func, &fwdResult);

            *result = fwdResult;
        }
        else
        {
            SPMod::IForward::ExecType execType = fwd->getExecType();

            for (const auto &pair : gModuleInterface->getPluginMngrCore()->getPluginsListCore())
            {
                std::shared_ptr<Plugin> plugin = pair.second;

                SourcePawn::IPluginFunction *func = plugin->getRuntime()->GetFunctionByName(fwd->getName());

                if (!func || !func->IsRunnable())
                    continue;

                cell_t fwdResult = 0;
                bool succeed = pushParamsToFunc(fwd, func, &fwdResult);

                if (!succeed || hasEnumFlag(execType, SPMod::IForward::ExecType::Ignore))
                    continue;

                if (*result < fwdResult)
                    *result = fwdResult;

                if (hasEnumFlag(execType, SPMod::IForward::ExecType::Stop) &&
                    fwdResult == static_cast<cell_t>(SPMod::IForward::ReturnValue::Stop))
                {
                    if (!hasEnumFlag(execType, SPMod::IForward::ExecType::Highest))
                        *result = fwdResult;

                    *stop = true;
                    break;
                }
            }
        }
    }

    void Cvar(const SPMod::ICvar *const cvar, const char *old_value, const char *new_value)
    {
        auto range = gCvarPluginsCallbacks.equal_range(const_cast<SPMod::ICvar *>(cvar));
        for (auto it = range.first; it != range.second; it++)
        {
            SourcePawn::IPluginFunction *func = it->second;

            if (!func || !func->IsRunnable())
                continue;

            func->PushCell(gCvarsHandlers.getKey(cvar));
            func->PushString(old_value);
            func->PushString(new_value);
            func->Execute(nullptr);
        }
    }

    void Menu(SPMod::IMenu *const menu, SPMod::IMenu::IItem *const item, SPMod::IPlayer *const player, void *data)
    {
        auto func = reinterpret_cast<SourcePawn::IPluginFunction *>(data);
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

    void MenuText(SPMod::IMenu *const menu, int key, SPMod::IPlayer *const player, void *data)
    {
        auto func = reinterpret_cast<SourcePawn::IPluginFunction *>(data);
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
                                                 void *data)
    {
        auto func = reinterpret_cast<SourcePawn::IPluginFunction *>(data);
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

    SPMod::IForward::ReturnValue CmdCallback(SPMod::IPlayer *const player, const SPMod::ICommand *const cmd, void *data)
    {
        SourcePawn::IPluginFunction *spFunc = reinterpret_cast<SourcePawn::IPluginFunction *>(data);
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

    bool TimerCallback(SPMod::ITimer *const timer, void *data)
    {
        auto func = reinterpret_cast<SourcePawn::IPluginFunction *>(data);
        if (func && func->IsRunnable())
        {
            cell_t result = 0;
            func->PushCell(gTimerHandlers.getKey(timer));
            func->PushCell(*reinterpret_cast<cell_t *>(timer->getData()));
            func->Execute(&result);

            return static_cast<SPMod::IForward::ReturnValue>(result) != SPMod::IForward::ReturnValue::Stop;
        }

        return true;
    }

} // namespace SPExt::Listener
