/*
 *  Copyright (C) 2018-2020 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  SPMod is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with SPMod.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <public/IHelpers.hpp>
#include "ForwardSystem.hpp"

Forward::Param::Param(Type type) : m_dataType(type), m_copyback(false), m_stringFlags(Forward::StringFlags::None) {}

std::any Forward::Param::getData() const
{
    return m_data;
}

bool Forward::Param::shouldCopyback() const
{
    return m_copyback;
}

Forward::StringFlags Forward::Param::getStringFlags() const
{
    return m_stringFlags;
}

Forward::Param::Type Forward::Param::getDataType() const
{
    return m_dataType;
}

std::size_t Forward::Param::getDataSize() const
{
    return m_size;
}

void Forward::Param::setData(std::any data)
{
    m_data = data;
}

void Forward::Param::setSize(std::size_t size)
{
    m_size = size;
}

void Forward::Param::setCopyback(bool copyback)
{
    m_copyback = copyback;
}

void Forward::Param::setStringFlags(Forward::StringFlags stringFlags)
{
    m_stringFlags = stringFlags;
}

Forward::Forward(std::string_view name,
                 std::array<Forward::Param::Type, IForward::MAX_EXEC_PARAMS> paramstypes,
                 const std::vector<Forward::Callback> &callbacks)
    : m_name(name), m_paramsNum(0), m_callbacks(callbacks), m_currentPos(0)
{
    std::size_t i = 0;
    for (auto type : paramstypes)
    {
        m_params.at(i++) = std::make_unique<Param>(type);

        if (type != Param::Type::None)
        {
            m_paramsNum++;
        }
    }
}

std::string_view Forward::getName() const
{
    return m_name;
}

const std::array<std::unique_ptr<Forward::Param>, IForward::MAX_EXEC_PARAMS> &Forward::getParams() const
{
    return m_params;
}

bool Forward::isExecuted() const
{
    return m_exec;
}

bool Forward::pushInt(std::int32_t integer)
{
    if (m_currentPos >= m_paramsNum)
        return false;

    try
    {
        const std::unique_ptr<Param> &param = m_params.at(m_currentPos);
        if (param->getDataType() != Forward::Param::Type::Int)
            return false;

        param->setData(integer);
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }

    m_currentPos++;
    return true;
}

bool Forward::pushInt(std::int32_t *integer, bool copyback)
{
    if (m_currentPos >= m_paramsNum)
        return false;

    try
    {
        const std::unique_ptr<Param> &param = m_params.at(m_currentPos);
        if (param->getDataType() != (Forward::Param::Type::Int | Forward::Param::Type::Pointer))
            return false;

        param->setData(integer);
        param->setCopyback(copyback);
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
    if (m_currentPos >= m_paramsNum)
        return false;

    try
    {
        const std::unique_ptr<Param> &param = m_params.at(m_currentPos);
        if (param->getDataType() != Forward::Param::Type::Float)
            return false;

        param->setData(real);
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }

    m_currentPos++;
    return true;
}

bool Forward::pushFloat(float *real, bool copyback)
{
    if (m_currentPos >= m_paramsNum)
        return false;

    try
    {
        const std::unique_ptr<Param> &param = m_params.at(m_currentPos);
        if (param->getDataType() != (Forward::Param::Type::Float | Forward::Param::Type::Pointer))
            return false;

        param->setData(real);
        param->setCopyback(copyback);
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }

    m_currentPos++;
    return true;
}

bool Forward::pushArray(std::variant<std::int32_t *, float *> array, std::size_t size, bool copyback)
{
    if (m_currentPos >= m_paramsNum)
        return false;

    try
    {
        const std::unique_ptr<Param> &param = m_params.at(m_currentPos);
        if (param->getDataType() != Forward::Param::Type::Array)
            return false;

        param->setSize(size);
        param->setData(array);
        param->setCopyback(copyback);
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }

    m_currentPos++;
    return true;
}

bool Forward::pushString(std::string_view string)
{
    if (m_currentPos >= m_paramsNum)
        return false;

    try
    {
        const std::unique_ptr<Param> &param = m_params.at(m_currentPos);
        if (param->getDataType() != Forward::Param::Type::String)
            return false;

        param->setData(std::string(string));
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }

    m_currentPos++;
    return true;
}

bool Forward::pushString(char *buffer, std::size_t size, IForward::StringFlags sflags, bool copyback)
{
    if (m_currentPos >= m_paramsNum)
        return false;

    try
    {
        const std::unique_ptr<Param> &param = m_params.at(m_currentPos);
        if (param->getDataType() != Forward::Param::Type::String)
            return false;

        param->setData(buffer);
        param->setSize(size);
        param->setCopyback(copyback);
        param->setStringFlags(sflags);
    }
    catch (const std::out_of_range &e [[maybe_unused]])
    {
        return false;
    }

    m_currentPos++;
    return true;
}

std::array<IForward::IParam *, IForward::MAX_EXEC_PARAMS> Forward::getParamsImpl() const
{
    std::array<IForward::IParam *, IForward::MAX_EXEC_PARAMS> params = {};

    std::size_t idx = 0;
    for (const auto &param : m_params)
    {
        /* No more params */
        if (!param)
        {
            break;
        }

        params.at(idx++) = param.get();
    }

    return params;
}

bool MultiForward::execFunc(std::int32_t *result)
{
    // If passed number of passed arguments is lower then required one fail now
    if (m_paramsNum > m_currentPos)
        return false;

    m_exec = true;

    std::int32_t returnValue = 0;
    bool stop = false;
    for (const auto &callback : m_callbacks)
    {
        callback(this, returnValue, stop);

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
                           std::array<Forward::Param::Type, MAX_EXEC_PARAMS> paramstypes,
                           ExecType type,
                           const std::vector<Forward::Callback> &callbacks)
    : Forward(name, paramstypes, callbacks), m_execType(type)
{
}

IPlugin *MultiForward::getPlugin() const
{
    return nullptr;
}

Forward::ExecType MultiForward::getExecType() const
{
    return m_execType;
}

SingleForward::SingleForward(std::string_view name,
                             std::array<Forward::Param::Type, MAX_EXEC_PARAMS> paramstypes,
                             IPlugin *plugin,
                             const std::vector<Forward::Callback> &callbacks)
    : Forward(name, paramstypes, callbacks), m_plugin(plugin)
{
}

IPlugin *SingleForward::getPlugin() const
{
    return m_plugin;
}

Forward::ExecType SingleForward::getExecType() const
{
    return IForward::ExecType::Ignore;
}

bool SingleForward::execFunc(std::int32_t *result)
{
    // If passed number of passed arguments is lower then required one fail now
    if (m_paramsNum > m_currentPos)
        return false;

    m_exec = true;

    // TODO: Execute the right callback except all of them
    std::int32_t returnValue = 0;
    static bool dummy;
    for (const auto &callback : m_callbacks)
        callback(this, returnValue, dummy);

    (void)dummy;

    if (result)
        *result = returnValue;

    m_exec = false;
    m_currentPos = 0;

    return true;
}

Forward *ForwardMngr::createForward(std::string_view name,
                                    Forward::ExecType exec,
                                    std::array<Forward::Param::Type, IForward::MAX_EXEC_PARAMS> params,
                                    IPlugin *plugin)
{
    if (!plugin) // Global forward
    {
        // Global forward with the same name already found
        if (size_t count = m_forwards.count(name.data()); count)
            return nullptr;

        return m_forwards.emplace(name.data(), std::make_unique<MultiForward>(name, params, exec, m_callbacks))
            ->second.get();
    }

    // Check if forward with the same name and for the same plugin is already registered
    for (auto iter = m_forwards.find(name.data()); iter != m_forwards.end(); iter++)
    {
        if (iter->second->getPlugin() == plugin)
            return nullptr;
    }

    return m_forwards.emplace(name.data(), std::make_unique<SingleForward>(name, params, plugin, m_callbacks))
        ->second.get();
}

void ForwardMngr::addForwardListener(Forward::Callback func)
{
    m_callbacks.emplace_back(func);
}

void ForwardMngr::clearForwards()
{
    m_forwards.clear();
    m_callbacks.clear();
}

bool ForwardMngr::deleteForward(const IForward *forward)
{
    const IPlugin *plugin = forward->getPlugin();

    auto iter = m_forwards.find(forward->getName().data());
    while (iter != m_forwards.end())
    {
        if (iter->second->isExecuted())
        {
            iter++;
            continue;
        }

        if (!plugin)
        {
            m_forwards.erase(iter);
            return true;
        }
        else if (plugin == iter->second->getPlugin())
        {
            m_forwards.erase(iter);
            return true;
        }
        iter++;
    }

    return false;
}
