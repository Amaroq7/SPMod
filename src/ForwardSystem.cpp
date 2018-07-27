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

const char *Forward::getName() const
{
    return m_name.c_str();
}

std::size_t Forward::getId() const
{
    return m_id;
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

MultiForward::MultiForward(std::string_view name,
                           std::size_t id,
                           std::array<IForward::ParamType, SP_MAX_EXEC_PARAMS> paramstypes,
                           std::size_t params,
                           ExecType type) : m_execType(type)
{
    m_name = name;
    m_id = id;
    m_paramTypes = paramstypes;
    m_currentPos = 0;
    m_paramsNum = params;
}

IPlugin *MultiForward::getPlugin() const
{
    return nullptr;
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
                             std::size_t size,
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
                                std::size_t size,
                                IForward::StringFlags sflags,
                                bool copyback)
{
    try
    {
        if (m_paramTypes.at(m_currentPos) != ParamType::String)
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
    if (m_paramsNum > m_currentPos)
        return false;

    m_exec = true;
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

    m_exec = false;
    m_currentPos = 0;
    return true;
}

void MultiForward::pushParamsToFunction(SourcePawn::IPluginFunction *func)
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
}

void MultiForward::resetParams()
{
    m_currentPos = 0;
}

std::shared_ptr<Plugin> MultiForward::getPluginCore() const
{
    return nullptr;
}

SingleForward::SingleForward(std::string_view name,
                             std::size_t id,
                             std::array<IForward::ParamType, SP_MAX_EXEC_PARAMS> paramstypes,
                             std::size_t params,
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

IPlugin *SingleForward::getPlugin() const
{
    return m_plugin.lock().get();
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
                              std::size_t size,
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
                                 std::size_t size,
                                 IForward::StringFlags sflags,
                                 bool copyback)
{
    try
    {
        if (m_paramTypes.at(m_currentPos) != ParamType::String)
            return false;

        auto szflags = static_cast<std::underlying_type_t<IForward::StringFlags>>(sflags);
        return m_pluginFunc->PushStringEx(buffer, size, szflags, copyback ? SM_PARAM_COPYBACK : 0) == SP_ERROR_NONE;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }
}

bool SingleForward::execFunc(cell_t *result)
{
    // If passed number of passed arguments is lower then required one fail now
    if (m_paramsNum > m_currentPos)
        return false;

    m_exec = true;
    bool succeed = m_pluginFunc->Execute(result) == SP_ERROR_NONE;
    m_exec = false;

    return succeed;
}

void SingleForward::resetParams()
{
    m_pluginFunc->Cancel();
}

std::shared_ptr<Plugin> SingleForward::getPluginCore() const
{
    return m_plugin.lock();
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
                                     std::size_t params,
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
                                     std::size_t params,
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

std::shared_ptr<Forward> ForwardMngr::findForward(std::size_t id) const
{
    for (auto pair : m_forwards)
    {
        if (pair.second->getId() == id)
            return pair.second;
    }

    return nullptr;
}

void ForwardMngr::addDefaultsForwards()
{
    using et = IForward::ExecType;
    using param = IForward::ParamType;
    std::array<IForward::ParamType, SP_MAX_EXEC_PARAMS> paramsList;

    auto defToId = [](FwdDefault forwardId)
    {
        return static_cast<std::size_t>(forwardId);
    };

    auto posId = defToId(FwdDefault::ClientConnect);
    paramsList = {{ param::Cell, param::String, param::String, param::String }};
    m_defaultForwards.at(posId) = createForwardCore("OnClientConnect", et::Stop, paramsList, 4);

    posId = defToId(FwdDefault::ClientDisconnect);
    paramsList = {{ param::Cell, param::Cell, param::String }};
    m_defaultForwards.at(posId) = createForwardCore("OnClientDisconnect", et::Ignore, paramsList, 3);
    
    posId = defToId(FwdDefault::ClientPutInServer);
    paramsList = {{ param::Cell }};
    m_defaultForwards.at(posId) = createForwardCore("OnClientPutInServer", et::Ignore, paramsList, 1);

    posId = defToId(FwdDefault::ClientCommmand);
    paramsList = {{ param::Cell }};
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
    std::array<IForward::ParamType, SP_MAX_EXEC_PARAMS> forwardParams = {};

    for (std::size_t i = 0; i < paramsnum; ++i)
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
                                                        std::size_t paramsnum,
                                                        std::shared_ptr<Plugin> plugin)
{
    std::shared_ptr<Forward> forward;

    // Global forward
    if (!plugin)
        forward = std::make_shared<MultiForward>(name, m_id, params, paramsnum, exec);
    // Forward for one plugin
    else
    {
        // Forward may be not found in plugin
        try
        {
            forward = std::make_shared<SingleForward>(name, m_id, params, paramsnum, plugin);
        }
        catch (const std::runtime_error &e [[maybe_unused]])
        {
            return nullptr;
        }
    }

    if (!m_forwards.try_emplace(name.data(), forward).second)
        return nullptr;

    m_id++;

    return forward;
}

void ForwardMngr::deleteForward(IForward *forward)
{
    m_forwards.erase(forward->getName());
}

std::size_t ForwardMngr::getForwardsNum() const
{
    return m_forwards.size();
}

void ForwardMngr::clearForwards()
{
    m_forwards.clear();
    m_id = 0;
}

void ForwardMngr::deleteForwardCore(std::shared_ptr<Forward> fwd)
{
    m_forwards.erase(fwd->getNameCore().data());
}

std::shared_ptr<Forward> ForwardMngr::getDefaultForward(ForwardMngr::FwdDefault fwd) const
{
    return m_defaultForwards.at(static_cast<std::size_t>(fwd)).lock();
}
