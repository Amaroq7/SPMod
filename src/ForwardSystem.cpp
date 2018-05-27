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

#include "ForwardSystem.hpp"

MultiForward::MultiForward(std::string_view name,
                           size_t id,
                           std::array<IForward::ParamType, SP_MAX_EXEC_PARAMS> paramstypes,
                           size_t params,
                           ExecType type) : m_execType(type)
{
    m_name = name;
    m_id = id;
    m_paramTypes = paramstypes;
    m_currentPos = 0;
    m_paramsNum = params;
}

bool MultiForward::pushCell(cell_t cell)
{
    try
    {
        if (m_paramTypes.at(m_currentPos) != ParamType::Cell)
            return false;

        ForwardParam &param = m_params.at(m_currentPos++);
        param.m_param = cell;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }

    return true;
}

bool MultiForward::pushCellPtr(cell_t *cell,
                               bool copyback)
{
    try
    {
        if (m_paramTypes.at(m_currentPos) != ParamType::CellRef)
            return false;

        ForwardParam &param = m_params.at(m_currentPos++);
        param.m_param = cell;
        param.m_copyback = copyback;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }

    return true;
}

bool MultiForward::pushFloat(float real)
{
    try
    {
        if (m_paramTypes.at(m_currentPos) != ParamType::Float)
            return false;

        ForwardParam &param = m_params.at(m_currentPos++);
        param.m_param = real;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }

    return true;
}

bool MultiForward::pushFloatPtr(float *real,
                                bool copyback)
{
    try
    {
        if (m_paramTypes.at(m_currentPos) != ParamType::FloatRef)
            return false;

        ForwardParam &param = m_params.at(m_currentPos++);
        param.m_param = real;
        param.m_copyback = copyback;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }

    return true;
}

bool MultiForward::pushArray(cell_t *array,
                             size_t size,
                             bool copyback)
{
    try
    {
        if (m_paramTypes.at(m_currentPos) != ParamType::Array)
            return false;

        ForwardParam &param = m_params.at(m_currentPos++);
        param.m_param = array;
        param.m_copyback = copyback;
        param.m_size = size;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }

    return true;
}

bool MultiForward::pushString(const char *string)
{
    try
    {
        if (m_paramTypes.at(m_currentPos) != ParamType::String)
            return false;

        ForwardParam &param = m_params.at(m_currentPos++);
        param.m_param = string;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }

    return true;
}

bool MultiForward::pushStringEx(char *buffer,
                                size_t size,
                                IForward::StringFlags sflags,
                                bool copyback)
{
    try
    {
        if (m_paramTypes.at(m_currentPos) != ParamType::StringEx)
            return false;

        ForwardParam &param = m_params.at(m_currentPos++);
        param.m_param = buffer;
        param.m_copyback = copyback;
        param.m_size = size;
        param.m_stringFlags = sflags;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }

    return true;
}

bool MultiForward::execFunc(cell_t *result)
{
    // If passed number of passed arguments is lower then required one fail now
    // May be pushed more, but they won't be passed either way
    if (m_paramsNum > m_currentPos)
        return false;

    cell_t tempResult = 0, returnValue = 0;
    for (const auto &pair : gSPGlobal->getPluginManagerCore()->getPluginsList())
    {
        std::shared_ptr<Plugin> plugin = pair.second;
        SourcePawn::IPluginFunction *funcToExecute = plugin->getRuntime()->GetFunctionByName(m_name.c_str());

        if (!funcToExecute)
            continue;

        // Pass params if there are any
        if (m_paramsNum)
            pushParamsToFunction(funcToExecute);

        funcToExecute->Execute(&tempResult);

        if (m_execType == ExecType::Ignore)
            continue;

        if (returnValue < tempResult)
            returnValue = tempResult;

        if (hasEnumFlag(m_execType, ExecType::Stop) && tempResult == ReturnValue::PluginStop)
        {
            if (!hasEnumFlag(m_execType, ExecType::Highest))
                returnValue = tempResult;

            break;
        }

    }
    if (m_execType != ExecType::Ignore)
        *result = returnValue;

    m_currentPos = 0;
    return true;
}

void MultiForward::pushParamsToFunction(SourcePawn::IPluginFunction *func)
{
    using sflags = IForward::StringFlags;
    std::variant<cell_t, cell_t *, float, float *, const char *, char *> paramVar;
    ForwardParam paramObj;

    for (size_t i = 0; i < m_paramsNum; ++i)
    {
        paramObj = m_params.at(i);
        paramVar = paramObj.m_param;

        switch (m_paramTypes.at(i))
        {
            case ParamType::Cell:
            {
                func->PushCell(std::get<cell_t>(paramVar));
                break;
            }
            case ParamType::CellRef:
            {
                int spFlags = (paramObj.m_copyback) ? SM_PARAM_COPYBACK : 0;
                func->PushCellByRef(std::get<cell_t *>(paramVar), spFlags);
                break;
            }
            case ParamType::Float:
            {
                func->PushFloat(std::get<float>(paramVar));
                break;
            }
            case ParamType::FloatRef:
            {
                int spFlags = (paramObj.m_copyback) ? SM_PARAM_COPYBACK : 0;
                func->PushFloatByRef(std::get<float *>(paramVar), spFlags);
                break;
            }
            case ParamType::Array:
            {
                int spFlags = (paramObj.m_copyback) ? SM_PARAM_COPYBACK : 0;
                func->PushArray(std::get<cell_t *>(paramVar), paramObj.m_size, spFlags);
                break;
            }
            case ParamType::String:
            {
                func->PushString(std::get<const char *>(paramVar));
                break;
            }
            case ParamType::StringEx:
            {
                int spFlags = (paramObj.m_copyback) ? SM_PARAM_COPYBACK : 0;
                auto spStringFlags = static_cast<std::underlying_type_t<sflags>>(paramObj.m_stringFlags);

                func->PushStringEx(std::get<char *>(paramVar), paramObj.m_size, spStringFlags, spFlags);
            }
            default:
                break;
        }
    }
}

SingleForward::SingleForward(std::string_view name,
                             size_t id,
                             std::array<IForward::ParamType, SP_MAX_EXEC_PARAMS> paramstypes,
                             size_t params,
                             std::shared_ptr<Plugin> plugin) : m_plugin(plugin)
{
    m_name = name;
    m_id = id;
    m_paramTypes = paramstypes;
    m_currentPos = 0;
    m_paramsNum = params;

    m_pluginFunc = plugin->getRuntime()->GetFunctionByName(name.data());
    if (!m_pluginFunc)
        throw std::runtime_error("Function not found!");
}

bool SingleForward::pushCell(cell_t cell)
{
    try
    {
        if (m_paramTypes.at(m_currentPos) != ParamType::Cell)
            return false;

        return m_pluginFunc->PushCell(cell) == SP_ERROR_NONE;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }
}

bool SingleForward::pushCellPtr(cell_t *cell,
                                bool copyback)
{
    try
    {
        if (m_paramTypes.at(m_currentPos) != ParamType::CellRef)
            return false;

        return m_pluginFunc->PushCellByRef(cell, copyback ? SM_PARAM_COPYBACK : 0) == SP_ERROR_NONE;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }
}

bool SingleForward::pushFloat(float real)
{
    try
    {
        if (m_paramTypes.at(m_currentPos) != ParamType::Float)
            return false;

        return m_pluginFunc->PushFloat(real) == SP_ERROR_NONE;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }
}

bool SingleForward::pushFloatPtr(float *real,
                                 bool copyback)
{
    try
    {
        if (m_paramTypes.at(m_currentPos) != ParamType::FloatRef)
            return false;

        return m_pluginFunc->PushFloatByRef(real, copyback ? SM_PARAM_COPYBACK : 0) == SP_ERROR_NONE;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }
}

bool SingleForward::pushArray(cell_t *array,
                              size_t size,
                              bool copyback)
{
    try
    {
        if (m_paramTypes.at(m_currentPos) != ParamType::Array)
            return false;

        return m_pluginFunc->PushArray(array, size, copyback ? SM_PARAM_COPYBACK : 0) == SP_ERROR_NONE;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }
}

bool SingleForward::pushString(const char *string)
{
    try
    {
        if (m_paramTypes.at(m_currentPos) != ParamType::String)
            return false;

        return m_pluginFunc->PushString(string) == SP_ERROR_NONE;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }
}

bool SingleForward::pushStringEx(char *buffer,
                                 size_t size,
                                 IForward::StringFlags sflags,
                                 bool copyback)
{
    try
    {
        if (m_paramTypes.at(m_currentPos) != ParamType::StringEx)
            return false;

        auto szflags = static_cast<std::underlying_type_t<IForward::StringFlags>>(sflags);
        return m_pluginFunc->PushStringEx(buffer, size, szflags, copyback ? SM_PARAM_COPYBACK : 0) == SP_ERROR_NONE;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }

    return true;
}

bool SingleForward::execFunc(cell_t *result)
{
    // If passed number of passed arguments is lower then required one fail now
    // May be pushed more, but they won't be passed either way
    if (m_paramsNum > m_currentPos)
        return false;

    return m_pluginFunc->Execute(result) == SP_ERROR_NONE;
}

const ForwardList *ForwardMngr::getForwardsList() const
{
    if (!getForwardsNum())
        return nullptr;

    auto *begin = new ForwardList;

    ForwardList *list = begin;
    for (const auto &pair : m_forwards)
    {
        list->forward = pair.second.get();
        list = list->next = new ForwardList;
    }
    list->next = nullptr;

    // Remove empty last element
    list = begin;
    while (list)
    {
        // Check if the next is last element
        if (!list->next->next)
        {
            delete list->next;
            list->next = nullptr;
        }
        list = list->next;
    }

    return begin;
}

void ForwardMngr::freeForwardsList(const ForwardList *list) const
{
    while (list)
    {
        const ForwardList *toDelete = list;
        list = list->next;
        delete toDelete;
    }
}

IForward *ForwardMngr::createForward(const char *name,
                                     IForward::ExecType exec,
                                     size_t params,
                                     ...)
{
    if (params > SP_MAX_EXEC_PARAMS)
        return nullptr;

    // Get passed params types
    std::va_list paramsList;
    va_start(paramsList, params);
    std::shared_ptr<Forward> createdForward = _createForwardVa(name, exec, paramsList, params);
    va_end(paramsList);

    return createdForward.get();
}

IForward *ForwardMngr::createForward(const char *name,
                                     IPlugin *plugin,
                                     size_t params,
                                     ...)
{
    using et = IForward::ExecType;

    if (params > SP_MAX_EXEC_PARAMS)
        return nullptr;

    // Get passed params types
    std::va_list paramsList;
    va_start(paramsList, params);
    std::shared_ptr<Forward> createdForward = _createForwardVa(name, et::Ignore, paramsList, params, plugin);
    va_end(paramsList);

    return createdForward.get();
}

IForward *ForwardMngr::findForward(const char *name) const
{
    return findForwardCore(name).get();
}

IForward *ForwardMngr::findForward(size_t id) const
{
    return findForwardCore(id).get();
}

std::shared_ptr<Forward> ForwardMngr::findForwardCore(std::string_view name) const
{
    auto pair = m_forwards.find(name.data());

    if (pair != m_forwards.end())
        return pair->second;

    return nullptr;
}

std::shared_ptr<Forward> ForwardMngr::findForwardCore(size_t id) const
{
    for (auto pair = m_forwards.begin(); pair != m_forwards.end(); pair++)
    {
        if (pair->second->getId() == id)
            return pair->second;
    }

    return nullptr;
}

void ForwardMngr::addDefaultsForwards()
{
    using et = IForward::ExecType;
    using param = IForward::ParamType;
    std::array<IForward::ParamType, SP_MAX_EXEC_PARAMS> paramsList;

    paramsList = { param::Cell, param::String, param::String, param::StringEx };
    createForwardCore("OnClientConnect", et::Stop, paramsList, 4);

    paramsList = { param::Cell, param::Cell, param::String };
    createForwardCore("OnClientDisconnect", et::Ignore, paramsList, 3);

    paramsList = { param::String, param::String, param::String, param::Float };
    createForwardCore("OnCvarChange", et::Ignore, paramsList, 4);

    paramsList = { };
    createForwardCore("OnPluginsLoaded", et::Ignore, paramsList, 0);
    createForwardCore("OnPluginInit", et::Ignore, paramsList, 0);
    createForwardCore("OnPluginEnd", et::Ignore, paramsList, 0);
    createForwardCore("OnPluginNatives", et::Ignore, paramsList, 0);
}

std::shared_ptr<Forward> ForwardMngr::_createForwardVa(std::string_view name,
                                                       IForward::ExecType exec,
                                                       std::va_list params,
                                                       size_t paramsnum,
                                                       IPlugin *plugin)
{
    std::array<IForward::ParamType, SP_MAX_EXEC_PARAMS> forwardParams;

    for (size_t i = 0; i < paramsnum; ++i)
        forwardParams.at(i) = static_cast<IForward::ParamType>(va_arg(params, int));

    // Find shared_ptr
    std::shared_ptr<Plugin> sharedPlugin;
    if (plugin)
    {
        const std::unique_ptr<PluginMngr> &plMngr = gSPGlobal->getPluginManagerCore();
        sharedPlugin = plMngr->getPluginCore(plugin->getIndentity());
    }

    return createForwardCore(name, exec, forwardParams, paramsnum, sharedPlugin);
}

std::shared_ptr<Forward> ForwardMngr::createForwardCore(std::string_view name,
                                                        IForward::ExecType exec,
                                                        std::array<IForward::ParamType, SP_MAX_EXEC_PARAMS> params,
                                                        size_t paramsnum,
                                                        std::shared_ptr<Plugin> plugin)
{
    static size_t id = 0;
    std::shared_ptr<Forward> forward;

    // Global forward
    if (!plugin)
        forward = std::make_shared<MultiForward>(name, id, params, paramsnum, exec);
    // Forward for one plugin
    else
    {
        // Forward may be not found in plugin
        try
        {
            forward = std::make_shared<SingleForward>(name, id, params, paramsnum, plugin);
        }
        catch (const std::runtime_error &e [[maybe_unused]])
        {
            return nullptr;
        }
    }

    if (!m_forwards.try_emplace(name.data(), forward).second)
        return nullptr;

    id++;

    return forward;
}