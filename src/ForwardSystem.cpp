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

    if (m_paramTypes.at(m_currentPos) != ParamType::CELL)
        return false;

    ForwardParam param;
    param.m_param = cell;

    m_params.at(m_currentPos++) = param;
    return true;
}

bool Forward::pushCellPtr(cell_t *cell, bool copyback)
{
    if (m_currentPos >= SP_MAX_EXEC_PARAMS)
        return false;

    if (m_paramTypes.at(m_currentPos) != ParamType::CELL_REF)
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

    if (m_paramTypes.at(m_currentPos) != ParamType::FLOAT)
        return false;

    ForwardParam param;
    param.m_param = real;

    m_params.at(m_currentPos++) = param;
    return true;
}

bool Forward::pushFloatPtr(float *real, bool copyback)
{
    if (m_currentPos >= SP_MAX_EXEC_PARAMS)
        return false;

    if (m_paramTypes.at(m_currentPos) != ParamType::FLOAT_REF)
        return false;

    ForwardParam param;
    param.m_param = real;
    param.m_copyback = copyback;

    m_params.at(m_currentPos++) = param;
    return true;
}

bool Forward::pushArray(cell_t *array, size_t size, bool copyback)
{
    if (m_currentPos >= SP_MAX_EXEC_PARAMS)
        return false;

    if (m_paramTypes.at(m_currentPos) != ParamType::ARRAY)
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

    if (m_paramTypes.at(m_currentPos) != ParamType::STRING)
        return false;

    ForwardParam param;
    param.m_param = string;

    m_params.at(m_currentPos++) = param;
    return true;
}

bool Forward::pushStringEx(char *buffer, size_t size, StringFlags sflags, bool copyback)
{
    if (m_currentPos >= SP_MAX_EXEC_PARAMS)
        return false;

    if (m_paramTypes.at(m_currentPos) != ParamType::STRINGEX)
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
    if (m_plugin)
    {
        auto *pluginRuntime = m_plugin->getRuntime();
        auto *funcToExecute = pluginRuntime->GetFunctionByName(m_name.c_str());

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
        auto *pluginManager = static_cast<PluginMngr *>(gSPGlobal->getPluginManager());
        cell_t tempResult = 0, returnValue = 0;
        for (const auto &entry : pluginManager->getPluginsList())
        {
            auto plugin = entry.second;
            auto *funcToExecute = plugin->getRuntime()->GetFunctionByName(m_name.c_str());

            if (!funcToExecute)
                continue;

            // Pass params if there are any
            if (m_paramsNum)
                pushParamsToFunction(funcToExecute);

            funcToExecute->Execute(&tempResult);

            if (m_execType & ExecType::IGNORE)
                continue;

            if (returnValue < tempResult)
                returnValue = tempResult;

            if (m_execType & ExecType::STOP && tempResult == ReturnValue::PLUGIN_STOP)
            {
                if (!(m_execType & ExecType::HIGHEST))
                    returnValue = tempResult;

                break;
            }

        }
        if (m_execType != ExecType::IGNORE)
            *result = returnValue;
    }

    m_currentPos = 0;
    return true;
}

void Forward::resetParams()
{
    m_currentPos = 0;
    // Clear array of pushed params
    m_params.fill(ForwardParam());
}

void Forward::pushParamsToFunction(SourcePawn::IPluginFunction *func)
{
    for (auto i = 0U; i < m_paramsNum; ++i)
    {
        auto paramObj = m_params.at(i);
        auto paramVar = paramObj.m_param;
        switch (m_paramTypes.at(i))
        {
            case ParamType::CELL:
            {
                func->PushCell(std::get<cell_t>(paramVar));
                break;
            }
            case ParamType::CELL_REF:
            {
                auto spFlags = (paramObj.m_copyback) ? SM_PARAM_COPYBACK : 0;
                func->PushCellByRef(std::get<cell_t *>(paramVar), spFlags);
                break;
            }
            case ParamType::FLOAT:
            {
                func->PushFloat(std::get<float>(paramVar));
                break;
            }
            case ParamType::FLOAT_REF:
            {
                auto spFlags = (paramObj.m_copyback) ? SM_PARAM_COPYBACK : 0;
                func->PushFloatByRef(std::get<float *>(paramVar), spFlags);
                break;
            }
            case ParamType::ARRAY:
            {
                auto spFlags = (paramObj.m_copyback) ? SM_PARAM_COPYBACK : 0;
                func->PushArray(std::get<cell_t *>(paramVar), paramObj.m_size, spFlags);
                break;
            }
            case ParamType::STRING:
            {
                func->PushString(std::get<const char *>(paramVar));
                break;
            }
            case ParamType::STRINGEX:
            {
                auto spFlags = (paramObj.m_copyback) ? SM_PARAM_COPYBACK : 0;
                auto spStringFlags = 0;
                if (paramObj.m_stringFlags != StringFlags::NONE)
                {
                    if (paramObj.m_stringFlags & StringFlags::UTF8)
                        spStringFlags |= SM_PARAM_STRING_UTF8;

                    if (paramObj.m_stringFlags & StringFlags::COPY)
                        spStringFlags |= SM_PARAM_STRING_COPY;

                    if (paramObj.m_stringFlags & StringFlags::BINARY)
                        spStringFlags |= SM_PARAM_STRING_BINARY;
                }

                func->PushStringEx(std::get<char *>(paramVar), paramObj.m_size, spStringFlags, spFlags);
            }
        }
    }
}

IForward *ForwardMngr::createForward(const char *name,
                                        IForward::ExecType exec,
                                        IPlugin *plugin,
                                        size_t params,
                                        ...)
{
    if (params > SP_MAX_EXEC_PARAMS)
        return nullptr;

    std::array<IForward::ParamType, SP_MAX_EXEC_PARAMS> forwardParams;

    // Get passed params types
    va_list paramsList;
    va_start(paramsList, params);

    for (auto i = 0U; i < params; ++i)
        forwardParams.at(i) = static_cast<IForward::ParamType>(va_arg(paramsList, int));

    va_end(paramsList);

    auto forwardPtr = std::make_shared<Forward>(name,
                                                forwardParams,
                                                params,
                                                exec,
                                                reinterpret_cast<Plugin *>(plugin));

    auto result = m_forwards.insert(std::make_pair(name, forwardPtr));

    if (!result.second)
        return nullptr;

    return forwardPtr.get();
}

IForward *ForwardMngr::findForward(const char *name)
{
    auto iter = m_forwards.find(name);

    if (iter != m_forwards.end())
        return iter->second.get();

    return nullptr;
}

Forward *ForwardMngr::createForward(const std::string &name,
                                    Plugin *plugin,
                                    Forward::ExecType exec,
                                    const std::initializer_list<Forward::ParamType> &params)
{
    auto paramsNum = params.size();

    if (paramsNum > SP_MAX_EXEC_PARAMS)
        return nullptr;

    std::copy(params.begin(), params.end(), m_paramsToPush.begin());

    auto forwardPtr = std::make_shared<Forward>(name,
                                                m_paramsToPush,
                                                paramsNum,
                                                exec,
                                                plugin);

    auto result = m_forwards.insert(std::make_pair(name, forwardPtr));

    if (!result.second)
        return nullptr;

    return forwardPtr.get();
}
