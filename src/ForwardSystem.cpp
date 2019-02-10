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
                 std::array<IForward::Param::Type, IForward::MAX_EXEC_PARAMS> paramstypes,
                 std::size_t params,
                 const std::vector<ForwardCallback> &callbacks) : m_name(name),
                                                                  m_paramsNum(params),
                                                                  m_callbacks(callbacks),
                                                                  m_currentPos(0)
{
    std::size_t i = 0;
    for (const auto &type : paramstypes)
    {
        IForward::Param &param = m_params.at(i++);
        param.m_dataType = type;
    }
}

const char *Forward::getName() const
{
    return m_name.c_str();
}

const IForward::Param *Forward::getParam(std::size_t id) const
{
    try
    {
        return &m_params.at(id);
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return nullptr;
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
        IForward::Param &param = m_params.at(m_currentPos);
        if (param.m_dataType != IForward::Param::Type::Int)
            return false;

        param.m_data = &integer;
        param.m_copyback = false;
        param.m_size = 0;
        param.m_stringFlags = StringFlags::None;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }

    m_currentPos++;

    return true;
}

bool Forward::pushInt(int *integer,
                      bool copyback)
{
    try
    {
        IForward::Param &param = m_params.at(m_currentPos);
        if (param.m_dataType != (IForward::Param::Type::Int |
                                 IForward::Param::Type::Pointer))
            return false;

        param.m_data = integer;
        param.m_copyback = copyback;
        param.m_size = 0;
        param.m_stringFlags = StringFlags::None;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }

    m_currentPos++;

    return true;
}

bool Forward::pushFloat(float real)
{
    try
    {
        IForward::Param &param = m_params.at(m_currentPos);
        if (param.m_dataType != IForward::Param::Type::Float)
            return false;

        param.m_data = &real;
        param.m_copyback = false;
        param.m_size = 0;
        param.m_stringFlags = StringFlags::None;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }

    m_currentPos++;

    return true;
}

bool Forward::pushFloat(float *real,
                        bool copyback)
{
    try
    {
        IForward::Param &param = m_params.at(m_currentPos);
        if (param.m_dataType != IForward::Param::Type::Float)
            return false;

        param.m_data = &real;
        param.m_copyback = copyback;
        param.m_size = 0;
        param.m_stringFlags = StringFlags::None;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }

    m_currentPos++;

    return true;
}

bool Forward::pushArray(void *array,
                        std::size_t size,
                        bool copyback)
{
    try
    {
        IForward::Param &param = m_params.at(m_currentPos);
        if (param.m_dataType != IForward::Param::Type::Array)
            return false;

        param.m_data = array;
        param.m_copyback = copyback;
        param.m_size = size;
        param.m_stringFlags = StringFlags::None;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }

    m_currentPos++;

    return true;
}

bool Forward::pushString(const char *string)
{
    try
    {
        IForward::Param &param = m_params.at(m_currentPos);
        if (param.m_dataType != IForward::Param::Type::String)
            return false;

        param.m_data = const_cast<char *>(string);
        param.m_size = strlen(string);
        param.m_copyback = false;
        param.m_stringFlags = StringFlags::None;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }

    m_currentPos++;

    return true;
}

bool Forward::pushString(char *buffer,
                         std::size_t size,
                         IForward::StringFlags sflags,
                         bool copyback)
{
    try
    {
        IForward::Param &param = m_params.at(m_currentPos);
        if (param.m_dataType != IForward::Param::Type::String)
            return false;

        param.m_data = buffer;
        param.m_copyback = copyback;
        param.m_size = size;
        param.m_stringFlags = sflags;
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }

    m_currentPos++;

    return true;
}

bool MultiForward::execFunc(int *result)
{
    // If passed number of passed arguments is lower then required one fail now
    if (m_paramsNum > m_currentPos)
        return false;

    m_exec = true;

    int returnValue = 0;
    bool stop = false;
    for (const auto &callback : m_callbacks)
    {
        callback(this, &returnValue, &stop);

        if (stop)
            break;
    }

    if (result)
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
                           std::array<IForward::Param::Type, MAX_EXEC_PARAMS> paramstypes,
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

IForward::ExecType MultiForward::getExecType() const
{
    return m_execType;
}

SingleForward::SingleForward(std::string_view name,
                             std::array<IForward::Param::Type, MAX_EXEC_PARAMS> paramstypes,
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

IForward::ExecType SingleForward::getExecType() const
{
    return IForward::ExecType::Ignore;
}

bool SingleForward::execFunc(int *result)
{
    // If passed number of passed arguments is lower then required one fail now
    if (m_paramsNum > m_currentPos)
        return false;

    m_exec = true;

    // TODO: Execute the right callback except all of them
    int returnValue = 0;
    for (const auto &callback : m_callbacks)
        callback(this, &returnValue, nullptr);

    if (result)
        *result = returnValue;

    m_exec = false;
    m_currentPos = 0;

    return true;
}

IForward *ForwardMngr::createForward(const char *name,
                                     IForward::ExecType exec,
                                     std::size_t paramsnum,
                                     IForward::Param::Type *params)
{
    if (paramsnum > Forward::MAX_EXEC_PARAMS)
        return nullptr;

    // Get passed params types
    std::shared_ptr<Forward> createdForward = _createForward(name, exec, params, paramsnum);

    return createdForward.get();
}

IForward *ForwardMngr::createForward(const char *name,
                                     IPlugin *plugin,
                                     std::size_t paramsnum,
                                     IForward::Param::Type *params)
{
    using et = IForward::ExecType;

    if (paramsnum > Forward::MAX_EXEC_PARAMS)
        return nullptr;

    // Get passed params types
    std::shared_ptr<Forward> createdForward = _createForward(name, et::Ignore, params, paramsnum, plugin);

    return createdForward.get();
}

void ForwardMngr::addDefaultsForwards()
{
    using et = IForward::ExecType;
    using param = IForward::Param::Type;
    std::array<IForward::Param::Type, Forward::MAX_EXEC_PARAMS> paramsList;

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

std::shared_ptr<Forward> ForwardMngr::_createForward(std::string_view name,
                                                     IForward::ExecType exec,
                                                     IForward::Param::Type *params,
                                                     std::size_t paramsnum,
                                                     IPlugin *plugin)
{
    std::array<IForward::Param::Type, IForward::MAX_EXEC_PARAMS> forwardParams;

    for (std::size_t i = 0; i < paramsnum; ++i)
        forwardParams.at(i) = static_cast<IForward::Param::Type>(params[i]);

    return createForwardCore(name, exec, forwardParams, paramsnum, plugin);
}

std::shared_ptr<Forward> ForwardMngr::createForwardCore(std::string_view name,
                                                        IForward::ExecType exec,
                                                        std::array<IForward::Param::Type, IForward::MAX_EXEC_PARAMS> params,
                                                        std::size_t paramsnum,
                                                        IPlugin *plugin)
{
    std::shared_ptr<Forward> forward;

    if (!plugin) // Global forward
        forward = std::make_shared<MultiForward>(name, params, paramsnum, exec, m_callbacks);
    else // Forward for one plugin
        forward = std::make_shared<SingleForward>(name, params, paramsnum, plugin, m_callbacks);

    if (!m_forwards.try_emplace(name.data(), forward).second)
        return nullptr;

    return forward;
}

void ForwardMngr::deleteForward(IForward *forward)
{
    m_forwards.erase(forward->getName());
}

void ForwardMngr::addForwardListener(ForwardCallback func)
{
    m_callbacks.push_back(func);
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
