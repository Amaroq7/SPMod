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

bool Forward::pushCell(cell_t cell)
{
    if (m_currentPos >= SP_MAX_EXEC_PARAMS)
        return false;

    if (m_paramTypes.at(m_currentPos) != ParamType::Cell)
        return false;

    ForwardParam param;
    param.m_param = cell;

    m_params.at(m_currentPos++) = param;
    return true;
}

bool Forward::pushCellPtr(cell_t *cell,
                            bool copyback)
{
    if (m_currentPos >= SP_MAX_EXEC_PARAMS)
        return false;

    if (m_paramTypes.at(m_currentPos) != ParamType::CellRef)
        return false;

    ForwardParam param;
    param.m_param = cell;
    param.m_copyback = copyback;

    m_params.at(m_currentPos++) = param;
    return true;
}

bool Forward::pushFloat(float real)
{
    if (m_currentPos >= SP_MAX_EXEC_PARAMS)
        return false;

    if (m_paramTypes.at(m_currentPos) != ParamType::Float)
        return false;

    ForwardParam param;
    param.m_param = real;

    m_params.at(m_currentPos++) = param;
    return true;
}

bool Forward::pushFloatPtr(float *real,
                            bool copyback)
{
    if (m_currentPos >= SP_MAX_EXEC_PARAMS)
        return false;

    if (m_paramTypes.at(m_currentPos) != ParamType::FloatRef)
        return false;

    ForwardParam param;
    param.m_param = real;
    param.m_copyback = copyback;

    m_params.at(m_currentPos++) = param;
    return true;
}

bool Forward::pushArray(cell_t *array,
                        size_t size,
                        bool copyback)
{
    if (m_currentPos >= SP_MAX_EXEC_PARAMS)
        return false;

    if (m_paramTypes.at(m_currentPos) != ParamType::Array)
        return false;

    ForwardParam param;
    param.m_param = array;
    param.m_copyback = copyback;
    param.m_size = size;

    m_params.at(m_currentPos++) = param;
    return true;
}

bool Forward::pushString(const char *string)
{
    if (m_currentPos >= SP_MAX_EXEC_PARAMS)
        return false;

    if (m_paramTypes.at(m_currentPos) != ParamType::String)
        return false;

    ForwardParam param;
    param.m_param = string;

    m_params.at(m_currentPos++) = param;
    return true;
}

bool Forward::pushStringEx(char *buffer,
                           size_t size,
                           StringFlags sflags,
                           bool copyback)
{
    if (m_currentPos >= SP_MAX_EXEC_PARAMS)
        return false;

    if (m_paramTypes.at(m_currentPos) != ParamType::StringEx)
        return false;

    ForwardParam param;
    param.m_param = buffer;
    param.m_copyback = copyback;
    param.m_size = size;
    param.m_stringFlags = sflags;

    m_params.at(m_currentPos++) = param;
    return true;
}

bool Forward::execFunc(cell_t *result)
{
    // If passed number of passed arguments is lower then required one fail now
    // May be pushed more, but they won't be passed either way
    if (m_paramsNum > m_currentPos)
        return false;

    // Forward for one plugin
    if (std::shared_ptr<Plugin> plugin = m_plugin.lock(); plugin)
    {
        SourcePawn::IPluginRuntime *pluginRuntime = plugin->getRuntime();
        SourcePawn::IPluginFunction *funcToExecute = pluginRuntime->GetFunctionByName(m_name.c_str());

        if (!funcToExecute)
            return false;

        // Pass params if there are any
        if (m_paramsNum)
            pushParamsToFunction(funcToExecute);

        if (funcToExecute->Execute(result) != SP_ERROR_NONE)
            return false;
    }
    // Forward for all plugins
    else
    {
        cell_t tempResult = 0, returnValue = 0;
        for (const auto &entry : gSPGlobal->getPluginManagerCore()->getPluginsList())
        {
            std::shared_ptr<Plugin> plugin = entry.second;
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
    }

    m_currentPos = 0;
    return true;
}

void Forward::resetParams()
{
    m_currentPos = 0;
}

void Forward::pushParamsToFunction(SourcePawn::IPluginFunction *func)
{
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
                int spStringFlags = 0;
                if (paramObj.m_stringFlags != StringFlags::None)
                {
                    if (hasEnumFlag(paramObj.m_stringFlags, StringFlags::Utf8))
                        spStringFlags |= SM_PARAM_STRING_UTF8;

                    if (hasEnumFlag(paramObj.m_stringFlags, StringFlags::Copy))
                        spStringFlags |= SM_PARAM_STRING_COPY;

                    if (hasEnumFlag(paramObj.m_stringFlags, StringFlags::Binary))
                        spStringFlags |= SM_PARAM_STRING_BINARY;
                }

                func->PushStringEx(std::get<char *>(paramVar), paramObj.m_size, spStringFlags, spFlags);
            }
        }
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
    va_list paramsList;
    va_start(paramsList, params);
    std::shared_ptr<Forward> createdForward = _createForwardVa(name, exec, paramsList, params);
    va_end(paramsList);

    return createdForward.get();
}

IForward *ForwardMngr::findForward(const char *name) const
{
    return findForwardCore(name).get();
}

std::shared_ptr<Forward> ForwardMngr::findForwardCore(std::string_view name) const
{
    auto iter = m_forwards.find(name.data());

    if (iter != m_forwards.end())
        return iter->second;

    return nullptr;
}

std::shared_ptr<Forward> ForwardMngr::createForwardCore(std::string_view name,
                                                        IForward::ExecType exec,
                                                        std::initializer_list<IForward::ParamType> params)
{
    size_t paramsNum = params.size();

    if (paramsNum > SP_MAX_EXEC_PARAMS)
        return nullptr;

    std::array<IForward::ParamType, SP_MAX_EXEC_PARAMS> forwardParams;
    std::copy(params.begin(), params.end(), forwardParams.begin());

    return _createForward(name, exec, forwardParams, paramsNum);
}

size_t ForwardMngr::getParamSize(size_t id)
{
    try
    {
        return m_preparedParams.at(id).m_size;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return 0;
    }
}

bool ForwardMngr::getParamCb(size_t id)
{
    try
    {
        return m_preparedParams.at(id).m_copyback;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }
}

IForward::StringFlags ForwardMngr::getParamSf(size_t id)
{
    try
    {
        return m_preparedParams.at(id).m_sflags;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return IForward::StringFlags::None;
    }
}

std::optional<size_t> ForwardMngr::addParam(std::any param,
                                            size_t size,
                                            bool copyback,
                                            IForward::StringFlags sflags)
{
    if (!param.has_value() || m_preparedParamsNum >= SP_MAX_EXEC_PARAMS)
        return std::nullopt;

    PreparedParam preparedParam;
    // Check for cell_t array first
    try
    {
        preparedParam.m_param = std::any_cast<cell_t *>(param);
    }
    catch (const std::bad_any_cast &e [[maybe_unused]])
    {
        // If fails get string array
        preparedParam.m_param = std::any_cast<char *>(param);
    }

    preparedParam.m_size = size;
    preparedParam.m_copyback = copyback;
    preparedParam.m_sflags = sflags;

    m_preparedParams.at(m_preparedParamsNum++) = preparedParam;

    return m_preparedParamsNum - 1;
}

void ForwardMngr::addDefaultsForwards()
{
    using et = IForward::ExecType;
    using param = IForward::ParamType;
    std::initializer_list<IForward::ParamType> paramsList;

    paramsList = { param::Cell, param::String, param::String, param::StringEx };
    createForwardCore("OnClientConnect", et::Stop, paramsList);

    paramsList = { param::Cell, param::Cell, param::String };
    createForwardCore("OnClientDisconnect", et::Ignore, paramsList);

    paramsList = { param::String, param::String, param::String, param::Float };
    createForwardCore("OnCvarChange", et::Ignore, paramsList);

    paramsList = { };
    createForwardCore("OnPluginsLoaded", et::Ignore, paramsList);
    createForwardCore("OnPluginInit", et::Ignore, paramsList);
    createForwardCore("OnPluginEnd", et::Ignore, paramsList);
    createForwardCore("OnPluginNatives", et::Ignore, paramsList);
}

std::shared_ptr<Forward> ForwardMngr::_createForwardVa(std::string_view name,
                                                       IForward::ExecType exec,
                                                       std::va_list params,
                                                       size_t paramsnum)
{
    std::array<IForward::ParamType, SP_MAX_EXEC_PARAMS> forwardParams;

    for (size_t i = 0; i < paramsnum; ++i)
        forwardParams.at(i) = static_cast<IForward::ParamType>(va_arg(params, int));

    return _createForward(name, exec, forwardParams, paramsnum);
}

std::shared_ptr<Forward> ForwardMngr::_createForward(std::string_view name,
                                                     IForward::ExecType exec,
                                                     std::array<IForward::ParamType, SP_MAX_EXEC_PARAMS> params,
                                                     size_t paramsnum)
{
    auto forward = std::make_shared<Forward>(name, params, paramsnum, exec);

    if (!m_forwards.try_emplace(name.data(), forward).second)
        return nullptr;

    return forward;
}