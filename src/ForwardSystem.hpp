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

#pragma once

#include <IForwardSystem.hpp>

using namespace SPMod;

/* @brief General forward used in SPMod.
 *        It can be either SingleForward or MutliForward.
 *        To distinguish them between them value returned from getPluginCore() needs to be checked against nullptr
 *        if it is nullptr then Forward is MultiForward, otherwise it is SingleForward.
 */
class Forward : public IForward
{
public:
    class Param final : public IParam
    {
    public:
        explicit Param(Type type);
        Param() = delete;
        Param(const Param &other) = delete;
        Param(Param &&other) = delete;
        ~Param() override = default;

        // IParam
        [[nodiscard]] Variants getData() const final;
        [[nodiscard]] bool shouldCopyback() const final;
        [[nodiscard]] IForward::StringFlags getStringFlags() const final;

        // Param
        void setData(Variants data);
        void setCopyback(bool copyback);
        void setStringFlags(IForward::StringFlags stringFlags);
        [[nodiscard]] Forward::Param::Type getDataType() const;

    private:
        Type m_dataType;
        Variants m_data;
        bool m_copyback = false;                     /* True if data is meant to be overwritten */
        IForward::StringFlags m_stringFlags = IForward::StringFlags::None; /* String flags */
    };

public:
    explicit Forward(std::array<Forward::Param::Type, IForward::MAX_EXEC_PARAMS> paramsTypes, ExecType execType);
    ~Forward() override = default;

    [[nodiscard]] std::string_view getName() const override;

    bool pushInt(std::int32_t integer) final;
    bool pushInt(std::int32_t *integer) final;

    bool pushFloat(float real) final;
    bool pushFloat(float *real) final;

    bool pushArray(std::vector<std::variant<std::int32_t, float>> &array) final;

    bool pushString(std::string_view string) final;
    bool pushString(std::string &string, IForward::StringFlags sflags, bool copyback) final;

    [[nodiscard]] ExecType getExecType() const override;

    void addFunction(IForward::Callback func) override;
    void removeFunction(IForward::Callback func) override;

    void resetParams() final;

    [[nodiscard]] bool isBeingExecuted() const;
    void setBeingExecuted(bool beingExecuted);
    [[nodiscard]] std::size_t getPushedParamsCount() const;
    [[nodiscard]] std::size_t getParamsCount() const;


protected:
    /* number of parameters in forward */
    std::size_t m_paramsNum;

    /* true if forward is being executed */
    bool m_beingExecuted = false;

    /* stores parameters */
    std::array<std::unique_ptr<Param>, MAX_EXEC_PARAMS> m_params;

    /* number of already pushed params */
    std::size_t m_currentPos;

    /* exec type of forward */
    ExecType m_execType = ExecType::Ignore;
};

class GlobalForward final : public Forward
{
public:
    GlobalForward(std::string_view name,
                  std::array<IForward::IParam::Type, MAX_EXEC_PARAMS> paramsTypes,
                  ExecType type);

    GlobalForward() = delete;
    GlobalForward(const GlobalForward &other) = delete;
    GlobalForward(GlobalForward &&other) = delete;
    ~GlobalForward() final = default;

    // IForward

    /* name of the forward */
    [[nodiscard]] std::string_view getName() const final;
    [[nodiscard]] Type getType() const final;
    void addPlugin(nstd::observer_ptr<IPlugin> plugin);
    [[nodiscard]] const std::vector<nstd::observer_ptr<IPlugin>> &getPlugins() const;

private:
    /* forward name */
    std::string m_name;

    std::vector<nstd::observer_ptr<IPlugin>> m_plugins;
};

class PrivateForward final : public Forward
{
public:
    PrivateForward(std::array<IForward::IParam::Type, MAX_EXEC_PARAMS> paramsTypes, ExecType execType);

    PrivateForward() = delete;
    PrivateForward(const PrivateForward &other) = delete;
    PrivateForward(PrivateForward &&other) = delete;
    ~PrivateForward() final = default;

    // IForward
    [[nodiscard]] Type getType() const final;
    [[nodiscard]] const std::vector<Callback> &getFunctions() const;
    void addFunction(IForward::Callback func) final;
    void removeFunction(IForward::Callback func) final;

private:
    /* functions to be executed */
    std::vector<Callback> m_functions;
};

class ForwardMngr final : public IForwardMngr
{
public:
    ForwardMngr() = default;
    ForwardMngr(const ForwardMngr &other) = delete;
    ForwardMngr(ForwardMngr &&other) = delete;
    ~ForwardMngr() final = default;
    ForwardMngr &operator=(const ForwardMngr &other) = delete;
    ForwardMngr &operator=(ForwardMngr &&other) = delete;

    // IForwardMngr
    nstd::observer_ptr<IForward> createForward(std::string_view name,
                                          Forward::ExecType exec,
                                          std::array<IForward::IParam::Type, IForward::MAX_EXEC_PARAMS> params) final;

    nstd::observer_ptr<IForward> createForward(
        IForward::ExecType exec,
        std::array<IForward::IParam::Type, IForward::MAX_EXEC_PARAMS> params
    ) final;

    bool execForward(
        nstd::observer_ptr<IForward> forward,
        IForward::Status *result
    ) final;

    bool deleteForward(nstd::observer_ptr<IForward> forward) final;

private:
    std::unordered_map<std::string, std::unique_ptr<Forward>> m_globalForwards;
    std::vector<std::unique_ptr<Forward>> m_privateForwards;
};
