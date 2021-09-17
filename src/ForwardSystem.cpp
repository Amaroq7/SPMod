/*
 *  Copyright (C) 2018-2021 SPMod Development Team
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

#include <anubis/IHelpers.hpp>
#include "ForwardSystem.hpp"
#include "SPGlobal.hpp"

Forward::Param::Param(Type type) : m_dataType(type) {}

IForward::IParam::Variants Forward::Param::getData() const
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

void Forward::Param::setData(Param::Variants data)
{
    m_data = data;
}

void Forward::Param::setCopyback(bool copyback)
{
    m_copyback = copyback;
}

void Forward::Param::setStringFlags(Forward::StringFlags stringFlags)
{
    m_stringFlags = stringFlags;
}

Forward::Forward(std::array<Forward::Param::Type, IForward::MAX_EXEC_PARAMS> paramsTypes, ExecType execType)
    : m_paramsNum(0), m_currentPos(0), m_execType(execType)
{
    std::size_t i = 0;
    for (auto type : paramsTypes)
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
    return {};
}

bool Forward::isBeingExecuted() const
{
    return m_beingExecuted;
}

void Forward::setBeingExecuted(bool beingExecuted)
{
    m_beingExecuted = beingExecuted;
}

bool Forward::pushInt(std::int32_t integer)
{
    if (m_currentPos >= m_paramsNum)
        return false;

    const auto &param = m_params[m_currentPos];
    if (param->getDataType() != Forward::Param::Type::Int)
        return false;

    param->setData(integer);

    m_currentPos++;
    return true;
}

bool Forward::pushInt(std::int32_t *integer)
{
    if (m_currentPos >= m_paramsNum)
        return false;

    const auto &param = m_params[m_currentPos];
    if (param->getDataType() != (Forward::Param::Type::Int | Forward::Param::Type::Pointer))
        return false;

    param->setData(integer);
    param->setCopyback(true);

    m_currentPos++;
    return true;
}

bool Forward::pushFloat(float real)
{
    if (m_currentPos >= m_paramsNum)
        return false;

    const auto &param = m_params[m_currentPos];
    if (param->getDataType() != Forward::Param::Type::Float)
        return false;

    param->setData(real);

    m_currentPos++;
    return true;
}

bool Forward::pushFloat(float *real)
{
    if (m_currentPos >= m_paramsNum)
        return false;

    const auto &param = m_params[m_currentPos];
    if (param->getDataType() != (Forward::Param::Type::Float | Forward::Param::Type::Pointer))
        return false;

    param->setData(real);
    param->setCopyback(true);

    m_currentPos++;
    return true;
}

bool Forward::pushArray(std::vector<std::variant<std::int32_t, float>> &array)
{
    if (m_currentPos >= m_paramsNum)
        return false;

    const auto &param = m_params[m_currentPos];
    if (param->getDataType() != Forward::Param::Type::Array)
        return false;

    param->setData(std::ref(array));
    param->setCopyback(true);

    m_currentPos++;
    return true;
}

bool Forward::pushString(std::string_view string)
{
    if (m_currentPos >= m_paramsNum)
        return false;

    const auto &param = m_params[m_currentPos];
    if (param->getDataType() != Forward::Param::Type::String)
        return false;

    param->setData(string);

    m_currentPos++;
    return true;
}

bool Forward::pushString(std::string &string, StringFlags sflags, bool copyback)
{
    if (m_currentPos >= m_paramsNum)
        return false;

    const auto &param = m_params[m_currentPos];
    if (param->getDataType() != Forward::Param::Type::String)
        return false;

    param->setData(std::ref(string));
    param->setCopyback(copyback);
    param->setStringFlags(sflags);

    m_currentPos++;
    return true;
}

Forward::ExecType Forward::getExecType() const
{
    return m_execType;
}

void Forward::addFunction(IForward::Callback func [[maybe_unused]]) {}
void Forward::removeFunction(IForward::Callback func [[maybe_unused]]) {}

void Forward::resetParams()
{
    m_currentPos = 0;
}

std::size_t Forward::getPushedParamsCount() const
{
    return m_currentPos;
}

std::size_t Forward::getParamsCount() const
{
    return m_paramsNum;
}

GlobalForward::GlobalForward(std::string_view name,
                             std::array<Forward::Param::Type, MAX_EXEC_PARAMS> paramsTypes,
                             ExecType type) : Forward(paramsTypes, type), m_name(name) {}

std::string_view GlobalForward::getName() const
{
    return m_name;
}

IForward::Type GlobalForward::getType() const
{
    return IForward::Type::Global;
}

void GlobalForward::addPlugin(nstd::observer_ptr<IPlugin> plugin)
{
    m_plugins.emplace_back(plugin);
}

const std::vector<nstd::observer_ptr<IPlugin>> &GlobalForward::getPlugins() const
{
    return m_plugins;
}

PrivateForward::PrivateForward(std::array<Forward::Param::Type, MAX_EXEC_PARAMS> paramsTypes, ExecType execType)
    : Forward(paramsTypes, execType) {}

const std::vector<IForward::Callback> &PrivateForward::getFunctions() const
{
    return m_functions;
}

IForward::Type PrivateForward::getType() const
{
    return IForward::Type::Private;
}

void PrivateForward::addFunction(IForward::Callback func)
{
    m_functions.emplace_back(func);
}

void PrivateForward::removeFunction(IForward::Callback func)
{
    auto iter = std::remove_if(m_functions.begin(), m_functions.end(), [&func](const IForward::Callback &fn) {
        return func.target<Status(*)(nstd::observer_ptr<IForward>)>() == fn.target<Status(*)(nstd::observer_ptr<IForward>)>();
    });
    m_functions.erase(iter, m_functions.end());
}

nstd::observer_ptr<IForward> ForwardMngr::createForward(std::string_view name,
                                    Forward::ExecType exec,
                                    std::array<Forward::Param::Type, IForward::MAX_EXEC_PARAMS> params)
{
    const auto [iter, inserted] = m_globalForwards.try_emplace(name.data(), std::make_unique<GlobalForward>(name, params, exec));
    if (!inserted)
    {
        return nullptr;
    }

    for (const auto &adapter : gSPGlobal->getAdaptersInterfaces())
    {
        for (const auto &plugin : adapter.second->getPluginMngr()->getPluginsList())
        {
            if (plugin && plugin->functionExist(name))
            {
                dynamic_cast<GlobalForward *>((iter->second.get()))->addPlugin(plugin);
            }
        }
    }

    return iter->second;
}

nstd::observer_ptr<IForward> ForwardMngr::createForward(IForward::ExecType exec,
                                                        std::array<IForward::IParam::Type, IForward::MAX_EXEC_PARAMS> params)
{
    return m_privateForwards.emplace_back(std::make_unique<PrivateForward>(params, exec));
}

bool ForwardMngr::deleteForward(nstd::observer_ptr<IForward> forward)
{
    if (forward)
    {
        return false;
    }

    auto forwardImpl = nstd::dynamic_observer_cast<Forward>(forward);
    if (forwardImpl && forwardImpl->isBeingExecuted())
    {
        return false;
    }

    if (forwardImpl->getType() == IForward::Type::Global)
    {
        m_globalForwards.erase(forwardImpl->getName().data());
    }
    else
    {
        auto iter = std::remove_if(m_privateForwards.begin(), m_privateForwards.end(), [&forwardImpl](const auto &fwd) {
             return fwd == forwardImpl;
        });

        m_privateForwards.erase(iter, m_privateForwards.end());
    }

    return true;
}

bool ForwardMngr::execForward(nstd::observer_ptr<IForward> forward, IForward::Status *result)
{
    if (forward)
    {
        return false;
    }

    auto forwardImpl{nstd::dynamic_observer_cast<Forward>(forward)};
    if (!forwardImpl)
    {
        return false;
    }

    // If passed number of passed arguments is lower then required one fail now
    if (forwardImpl->getParamsCount() > forwardImpl->getPushedParamsCount())
    {
        return false;
    }

    forwardImpl->setBeingExecuted(true);

    if (result)
    {
        *result = IForward::Status::Continue;
    }

    auto setupResult = [result](nstd::observer_ptr<Forward> fwd, IForward::Status returnValue) {
        switch (fwd->getExecType())
        {
            case IForward::ExecType::Single:
            {
                if (result)
                {
                    *result = returnValue;
                }
                return true;
            }
            case IForward::ExecType::Event:
            {
                if (result && *result < returnValue)
                {
                    *result = returnValue;
                }
                return true;
            }
            case IForward::ExecType::Hook:
            {
                if (result && *result < returnValue)
                {
                    *result = returnValue;
                }
                if (result && (*result == IForward::Status::Handled || *result == IForward::Status::Stop))
                {
                    return false;
                }
                return true;
            }
            default:
                return true;
        }
    };

    if (forwardImpl->getType() == Forward::Type::Private)
    {
        auto privateForward = nstd::dynamic_observer_cast<PrivateForward>(forwardImpl);
        for (const auto &function : privateForward->getFunctions())
        {
            IForward::Status returnValueTemp = function(privateForward);
            if (!setupResult(forwardImpl, returnValueTemp))
            {
                break;
            }
        }
    }
    else
    {
        auto globalForward = nstd::dynamic_observer_cast<GlobalForward>(forwardImpl);
        for (const auto &plugin : globalForward->getPlugins())
        {
            if (plugin)
            {
                continue;
            }

            IForward::Status returnValueTemp = plugin->functionCall(globalForward);
            if (!setupResult(forwardImpl, returnValueTemp))
            {
                break;
            }
        }
    }

    forwardImpl->setBeingExecuted(false);
    forwardImpl->resetParams();

    return true;
}
