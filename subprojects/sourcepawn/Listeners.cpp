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

namespace
{
    bool pushParamsToFunc(const IForward *const fwd,
                          SourcePawn::IPluginFunction *func,
                          cell_t *result)
    {
        if (!func || !func->IsRunnable())
            return false;

        for (std::size_t i = 0; i < fwd->getParamsNum(); i++)
        {
            const IForward::Param *param = fwd->getParam(i);
            IForward::Param::Type paramType = param->m_type;

            if (paramType & IForward::Param::Type::Int || 
                paramType & IForward::Param::Type::Float)
            {
                if (paramType & IForward::Param::Type::Array)
                    func->PushArray(reinterpret_cast<cell_t *>(param->m_data),
                                    m_data->m_size,
                                    (m_data->m_copyback ? SM_PARAM_COPYBACK : 0));

                else if (paramType & IForward::Param::Type::Pointer)
                {
                    if (paramType & IForward::Param::Type::Int)
                        func->PushCellByRef(reinterpret_cast<cell_t *>(param->m_data),
                                            (m_data->m_copyback ? SM_PARAM_COPYBACK : 0));
                    else
                        func->PushFloatByRef(reinterpret_cast<float *>(param->m_data),
                                            (m_data->m_copyback ? SM_PARAM_COPYBACK : 0));
                }
                else
                {
                    if (paramType & IForward::Param::Type::Int)
                        func->PushCell(*reinterpret_cast<cell_t *>(param->m_data));
                    else
                        func->PushFloat(*reinterpret_cast<float *>(param->m_data));
                }

            }
            else if (paramType & IForward::Param::Type::String)
            {
                if (param->m_copyback || param->m_stringFlags)
                {
                    int spStringFlags = 0;
                    IForward::StringFlags stringFlags = IForward::StringFlags::None;

                    if (stringFlags & IForward::StringFlags::Utf8)
                        spStringFlags |= SM_PARAM_STRING_UTF8;

                    if (stringFlags & IForward::StringFlags::Copy)
                        spStringFlags |= SM_PARAM_STRING_COPY;

                    if (stringFlags & IForward::StringFlags::Binary)
                        spStringFlags |= SM_PARAM_STRING_BINARY;

                    func->PushStringEx(reinterpret_cast<char *>(param->m_data),
                                        param->m_size,
                                        (m_data->m_copyback ? SM_PARAM_COPYBACK : 0),
                                        spStringFlags);
                }
                else
                {
                    func->PushString(reinterpret_cast<const char *>(param->m_data));
                }
            }
        }

        return func->Execute(&result) == SP_ERROR_NONE;
    }
}

namespace SPExt::Listener
{
    int Forward(const IForward *const fwd,
                int *result,
                bool *stop)
    {
        // One plugin forward
        std::shared_ptr<Plugin> plugin = gModuleInterface->getPluginMngrCore()->getPluginCore(fwd->getPlugin());
        if (plugin)
        {
            SourcePawn::IPluginFunction *func = plugin->getRuntime()->GetFunctionByName(fwd->getName());

            cell_t fwdResult = 0;
            pushParamsToFunc(fwd, func, &fwdResult);

            *result = fwdResult;
        }
        else
        {
            IForward::ExecType execType = fwd->getExecType();

            for (const auto &pair : gModuleInterface->getPluginMngrCore()->getPluginsList())
            {
                std::shared_ptr<Plugin> plugin = pair.second;

                SourcePawn::IPluginFunction *func = plugin->getRuntime()->GetFunctionByName(fwd->getName());

                cell_t fwdResult;
                bool succeed = pushParamsToFunc(fwd, func, &fwdResult);

                if (!succeed || hasEnumFlag(execType, ExecType::Ignore))
                    continue;

                if (*result < fwdResult)
                    *result = fwdResult;

                if (hasEnumFlag(execType, ExecType::Stop) && fwdResult == static_cast<cell_t>(ReturnValue::PluginStop))
                {
                    if (!hasEnumFlag(execType, ExecType::Highest))
                        *result = fwdResult;

                    *stop = true;
                    break;
                }
            }
        }
    }

    void Cvar(const ICvar *const cvar,
              const char *old_value,
              const char *new_value)
    {
        auto range = gCvarPluginsCallbacks.equal_range(cvar);
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

    void Menu(SPMod::IMenu *const menu, SPMod::IMenuItem *const item, SPMod::IPlayer *const player)
    {
        SourcePawn::IPluginFunction *func = gMenuPluginHandlers[menu];
        if(func && func->IsRunnable())
        {
            std::size_t menuId = gMenuHandlers.getKey(menu);
            func->PushCell(menuId);

            if(item->getNavType() == NavigationType::None)
            {
                func->PushCell(static_cast<cell_t>(menuPackItem(menuId, getItemIndex(item))));
            }
            else
            {
                func->PushCell(static_cast<cell_t>(item->getNavType()));
            }

            func->PushCell(static_cast<cell_t>(player->getIndex()));
            func->Execute(nullptr);
        }
    }

    void MenuText(SPMod::IMenu *const menu, int key, SPMod::IPlayer *const player)
    {
        SourcePawn::IPluginFunction *func = gMenuPluginHandlers[menu];
        if(func && func->IsRunnable())
        {
            func->PushCell(static_cast<cell_t>(gMenuHandlers.getKey(menu)));
            func->PushCell(static_cast<cell_t>(key));
            func->PushCell(static_cast<cell_t>(player->getIndex()));
            func->Execute(nullptr);
        }
    }

    ItemStatus MenuItemCallback(SPMod::IMenu *const menu, SPMod::IMenuItem *const item, SPMod::IPlayer *const player)
    {

    }
}
