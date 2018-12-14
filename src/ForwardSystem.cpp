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

Forward::Forward(std::string_view name,
                 std::array<IForward::ParamType, MAX_EXEC_PARAMS> paramstypes,
                 std::size_t params,
                 const std::vector<ForwardCallback> &callbacks) : m_name(name),
                                                                  m_paramTypes(paramstypes),
                                                                  m_paramsNum(params),
                                                                  m_callbacks(callbacks),
                                                                  m_currentPos(0)
{
}

const char *Forward::getName() const
{
    return m_name.c_str();
}

Forward::ParamType Forward::getParamType(std::size_t id) const
{
    try
    {
        return m_paramTypes.at(id);
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return ParamType::None;
    }
}

std::size_t Forward::getParamsNum() const
{
    return m_paramsNum;
}

std::string_view Forward::getNameCore() const
{
    return m_name;
}

bool Forward::isExecuted() const
{
    return m_exec;
}

bool Forward::pushInt(int integer)
{
    try
    {
        if (m_paramTypes.at(m_currentPos) != ParamType::Int)
            return false;

        Param &param = m_params.at(m_currentPos++);
        param.m_data = &integer;
        param.m_copyback = false;
        param.m_size = 0;
        param.m_stringFlags = StringFlags::None;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }

    return true;
}

bool Forward::pushIntPtr(int *integer,
                         bool copyback)
{
    try
    {
        if (m_paramTypes.at(m_currentPos) != ParamType::IntRef)
            return false;

        Param &param = m_params.at(m_currentPos++);
        param.m_data = integer;
        param.m_copyback = copyback;
        param.m_size = 0;
        param.m_stringFlags = StringFlags::None;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }

    return true;
}

bool Forward::pushFloat(float real)
{
    try
    {
        if (m_paramTypes.at(m_currentPos) != ParamType::Float)
            return false;

        Param &param = m_params.at(m_currentPos++);
        param.m_data = &real;
        param.m_copyback = false;
        param.m_size = 0;
        param.m_stringFlags = StringFlags::None;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }

    return true;
}

bool Forward::pushFloatPtr(float *real,
                           bool copyback)
{
    try
    {
        if (m_paramTypes.at(m_currentPos) != ParamType::FloatRef)
            return false;

        Param &param = m_params.at(m_currentPos++);
        param.m_data = &real;
        param.m_copyback = copyback;
        param.m_size = 0;
        param.m_stringFlags = StringFlags::None;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }

    return true;
}

bool Forward::pushArray(void *array,
                        std::size_t size,
                        bool copyback)
{
    try
    {
        if (m_paramTypes.at(m_currentPos) != ParamType::Array)
            return false;

        Param &param = m_params.at(m_currentPos++);
        param.m_data = array;
        param.m_copyback = copyback;
        param.m_size = size;
        param.m_stringFlags = StringFlags::None;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }

    return true;
}

bool Forward::pushString(const char *string)
{
    try
    {
        if (m_paramTypes.at(m_currentPos) != ParamType::String)
            return false;

        Param &param = m_params.at(m_currentPos++);
        param.m_data = const_cast<char *>(string);
        param.m_size = 0;
        param.m_copyback = false;
        param.m_stringFlags = StringFlags::None;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }

    return true;
}

bool Forward::pushStringEx(char *buffer,
                           std::size_t size,
                           IForward::StringFlags sflags,
                           bool copyback)
{
    try
    {
        if (m_paramTypes.at(m_currentPos) != ParamType::String)
            return false;

        Param &param = m_params.at(m_currentPos++);
        param.m_data = buffer;
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

bool Forward::pushData(void *data,
                       bool copyback)
{
    try
    {
        if (m_paramTypes.at(m_currentPos) != ParamType::String)
            return false;

        Param &param = m_params.at(m_currentPos++);
        param.m_data = data;
        param.m_copyback = copyback;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }

    return true;
}

bool MultiForward::execFunc(ReturnValue *result)
{
    // If passed number of passed arguments is lower then required one fail now
    if (m_paramsNum > m_currentPos)
        return false;

    m_exec = true;
    /*cell_t tempResult = 0, returnValue = 0;
    for (const auto &pair : gSPGlobal->getPluginManagerCore()->getPluginsList())
    {
        std::shared_ptr<Plugin> plugin = pair.second;
        SourcePawn::IPluginFunction *funcToExecute = plugin->getRuntime()->GetFunctionByName(m_name.c_str());

        if (!funcToExecute)
            continue;

        // Pass params if there are any
        if (m_paramsNum)
            pushParamsToFunction(funcToExecute);

        if (funcToExecute->Execute(&tempResult) != SP_ERROR_NONE)
        {
            m_exec = false;
            return false;
        }

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
    */

    ReturnValue returnValue;
    for (const auto &callback : m_callbacks)
        returnValue = callback(this, m_params.data());

    if (result && m_execType != ExecType::Ignore)
        *result = returnValue;

    m_exec = false;
    m_currentPos = 0;
    return true;
}

void Forward::resetParams()
{
    m_currentPos = 0;
}

MultiForward::MultiForward(std::string_view name,
                           std::array<IForward::ParamType, MAX_EXEC_PARAMS> paramstypes,
                           std::size_t params,
                           ExecType type,
                           const std::vector<ForwardCallback> callbacks) : Forward(name, paramstypes, params, callbacks),
                                                                           m_execType(type)
{
}

IPlugin *MultiForward::getPlugin() const
{
    return nullptr;
}

/*void MultiForward::pushParamsToFunction(SourcePawn::IPluginFunction *func)
{
    using sflags = IForward::StringFlags;
    std::variant<cell_t, cell_t *, float, float *, const char *, char *> paramVar;
    ForwardParam paramObj;

    for (std::size_t i = 0; i < m_paramsNum; ++i)
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
                try
                {
                    func->PushString(std::get<const char *>(paramVar));
                }
                catch (const std::bad_variant_access &e [[maybe_unused]])
                {
                    int spFlags = (paramObj.m_copyback) ? SM_PARAM_COPYBACK : 0;
                    auto spStringFlags = static_cast<std::underlying_type_t<sflags>>(paramObj.m_stringFlags);
                    func->PushStringEx(std::get<char *>(paramVar), paramObj.m_size, spStringFlags, spFlags);
                }
            }
            default:
                break;
        }
    }
}*/

SingleForward::SingleForward(std::string_view name,
                             std::array<IForward::ParamType, MAX_EXEC_PARAMS> paramstypes,
                             std::size_t params,
                             IPlugin *plugin,
                             const std::vector<ForwardCallback> &callbacks) : Forward(name, paramstypes, params, callbacks),
                                                                              m_plugin(plugin)
{
}

IPlugin *SingleForward::getPlugin() const
{
    return m_plugin;
}

bool SingleForward::execFunc(ReturnValue *result)
{
    // If passed number of passed arguments is lower then required one fail now
    if (m_paramsNum > m_currentPos)
        return false;

    m_exec = true;

    ReturnValue returnValue;
    for (const auto &callback : m_callbacks)
        returnValue = callback(this, m_params.data());

    if (result)
        *result = returnValue;

    m_exec = false;
    m_currentPos = 0;

    return true;
}

IForward *ForwardMngr::createForward(const char *name,
                                     IForward::ExecType exec,
                                     std::size_t params,
                                     ...)
{
    if (params > Forward::MAX_EXEC_PARAMS)
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
                                     std::size_t params,
                                     ...)
{
    using et = IForward::ExecType;

    if (params > Forward::MAX_EXEC_PARAMS)
        return nullptr;

    // Get passed params types
    std::va_list paramsList;
    va_start(paramsList, params);
    std::shared_ptr<Forward> createdForward = _createForwardVa(name, et::Ignore, paramsList, params, plugin);
    va_end(paramsList);

    return createdForward.get();
}

void ForwardMngr::addDefaultsForwards()
{
    using et = IForward::ExecType;
    using param = IForward::ParamType;
    std::array<IForward::ParamType, Forward::MAX_EXEC_PARAMS> paramsList;

    auto defToId = [](FwdDefault forwardId)
    {
        return static_cast<std::size_t>(forwardId);
    };

    auto posId = defToId(FwdDefault::ClientConnect);
    paramsList = {{ param::Int, param::String, param::String, param::String }};
    m_defaultForwards.at(posId) = createForwardCore("OnClientConnect", et::Stop, paramsList, 4);

    posId = defToId(FwdDefault::ClientDisconnect);
    paramsList = {{ param::Int, param::Int, param::String }};
    m_defaultForwards.at(posId) = createForwardCore("OnClientDisconnect", et::Ignore, paramsList, 3);
    
    posId = defToId(FwdDefault::ClientPutInServer);
    paramsList = {{ param::Int }};
    m_defaultForwards.at(posId) = createForwardCore("OnClientPutInServer", et::Ignore, paramsList, 1);

    posId = defToId(FwdDefault::ClientCommmand);
    paramsList = {{ param::Int }};
    m_defaultForwards.at(posId) = createForwardCore("OnClientCommand", et::Stop, paramsList, 1);

    posId = defToId(FwdDefault::MapChange);
    paramsList = {{ param::String }};
    m_defaultForwards.at(posId) = createForwardCore("OnMapChange", et::Stop, paramsList, 1);

    paramsList = { };

    posId = defToId(FwdDefault::PluginsLoaded);
    m_defaultForwards.at(posId) = createForwardCore("OnPluginsLoaded", et::Ignore, paramsList, 0);

    posId = defToId(FwdDefault::PluginInit);
    m_defaultForwards.at(posId) = createForwardCore("OnPluginInit", et::Ignore, paramsList, 0);

    posId = defToId(FwdDefault::PluginEnd);
    m_defaultForwards.at(posId) = createForwardCore("OnPluginEnd", et::Ignore, paramsList, 0);

    posId = defToId(FwdDefault::PluginNatives);
    m_defaultForwards.at(posId) = createForwardCore("OnPluginNatives", et::Ignore, paramsList, 0);
}

std::shared_ptr<Forward> ForwardMngr::_createForwardVa(std::string_view name,
                                                       IForward::ExecType exec,
                                                       std::va_list params,
                                                       std::size_t paramsnum,
                                                       IPlugin *plugin)
{
    std::array<IForward::ParamType, IForward::MAX_EXEC_PARAMS> forwardParams = {};

    for (std::size_t i = 0; i < paramsnum; ++i)
        forwardParams.at(i) = static_cast<IForward::ParamType>(va_arg(params, int));

    return createForwardCore(name, exec, forwardParams, paramsnum, plugin);
}

std::shared_ptr<Forward> ForwardMngr::createForwardCore(std::string_view name,
                                                        IForward::ExecType exec,
                                                        std::array<IForward::ParamType, IForward::MAX_EXEC_PARAMS> params,
                                                        std::size_t paramsnum,
                                                        IPlugin *plugin)
{
    std::shared_ptr<Forward> forward;

    // Global forward
    if (!plugin)
        forward = std::make_shared<MultiForward>(name, params, paramsnum, exec, m_callbacks);
    // Forward for one plugin
    else
    {
        // Forward may be not found in plugin
        try
        {
            forward = std::make_shared<SingleForward>(name, params, paramsnum, plugin, m_callbacks);
        }
        catch (const std::runtime_error &e [[maybe_unused]])
        {
            return nullptr;
        }
    }

    if (!m_forwards.try_emplace(name.data(), forward).second)
        return nullptr;

    return forward;
}

void ForwardMngr::deleteForward(IForward *forward)
{
    m_forwards.erase(forward->getName());
}

void ForwardMngr::clearForwards()
{
    m_forwards.clear();
}

void ForwardMngr::deleteForwardCore(std::shared_ptr<Forward> fwd)
{
    m_forwards.erase(fwd->getNameCore().data());
}

std::shared_ptr<Forward> ForwardMngr::getDefaultForward(ForwardMngr::FwdDefault fwd) const
{
    return m_defaultForwards.at(static_cast<std::size_t>(fwd)).lock();
}
