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
    class Param : public IParam
    {
    public:
        Param(Type type);
        Param() = delete;
        Param(const Param &other) = delete;
        Param(Param &&other) = delete;
        ~Param() = default;

        // IParam
        std::any getData() const override;
        bool shouldCopyback() const override;
        IForward::StringFlags getStringFlags() const override;
        Type getDataType() const override;
        std::size_t getDataSize() const override;

        // Param
        void setData(std::any data);
        void setSize(std::size_t size);
        void setCopyback(bool copyback);
        void setStringFlags(IForward::StringFlags stringFlags);

    private:
        Type m_dataType;
        std::any m_data;
        std::size_t m_size;
        bool m_copyback;                     /* True if data is meant to be overwritten */
        IForward::StringFlags m_stringFlags; /* String flags */
    };

public:
    Forward(std::string_view name,
            std::array<Forward::Param::Type, IForward::MAX_EXEC_PARAMS> paramstypes,
            const std::vector<Forward::Callback> &callbacks);

    virtual ~Forward() = default;

    /* name of the forward */
    std::string_view getName() const override;

    /* param of the forward */
    const std::array<std::unique_ptr<Param>, IForward::MAX_EXEC_PARAMS> &getParams() const;

    bool pushInt(std::int32_t integer) override;
    bool pushInt(std::int32_t *integer, bool copyback) override;

    bool pushFloat(float real) override;
    bool pushFloat(float *real, bool copyback) override;

    bool pushArray(std::variant<std::int32_t *, float *> array, std::size_t size, bool copyback) override;

    bool pushString(std::string_view string) override;
    bool pushString(char *buffer, std::size_t size, IForward::StringFlags sflags, bool copyback) override;

    void resetParams() override;

    bool isExecuted() const;

protected:
    std::array<IParam *, MAX_EXEC_PARAMS> getParamsImpl() const override;

protected:
    /* forward name */
    std::string m_name;

    /* number of parameters in forward */
    std::size_t m_paramsNum;

    /* true if forward is being executed */
    bool m_exec;

    /* stores parameters */
    std::array<std::unique_ptr<Param>, MAX_EXEC_PARAMS> m_params;

    /* extensions' callbacks */
    const std::vector<Forward::Callback> &m_callbacks;

    /* number of already pushed params */
    std::size_t m_currentPos;
};

/*
 * @brief Forward type which is executed in every loaded plugin.
 *        push* functions push params to cache. When calling execFunc()
 *        function is searched in a plugin, if it is found, parameters
 *        are read from cache and are pushed to SourcePawn::IPluginFunction.
 */

class MultiForward final : public Forward
{
public:
    MultiForward(std::string_view name,
                 std::array<IForward::IParam::Type, MAX_EXEC_PARAMS> paramstypes,
                 ExecType type,
                 const std::vector<Forward::Callback> &callbacks);

    MultiForward() = delete;
    MultiForward(const MultiForward &other) = delete;
    MultiForward(MultiForward &&other) = delete;
    ~MultiForward() = default;

    // IForward
    IPlugin *getPlugin() const override;
    ExecType getExecType() const override;

    bool execFunc(std::int32_t *result) override;

private:
    /* exec type of forward */
    ExecType m_execType;
};

/*
 * @brief Forward type which is executed in only one plugin.
 *        push* functions work a bit different than MultiForward ones
 *        since they push parameters directly to SourcePawn::IPluginFunction
 *        SingleForward guarantees that the function, that is going to be executed
 *        in the plugin, exists.
 */

class SingleForward final : public Forward
{
public:
    SingleForward(std::string_view name,
                  std::array<IForward::IParam::Type, MAX_EXEC_PARAMS> paramstypes,
                  IPlugin *plugin,
                  const std::vector<Forward::Callback> &callbacks);

    SingleForward() = delete;
    SingleForward(const SingleForward &other) = delete;
    SingleForward(SingleForward &&other) = delete;
    ~SingleForward() = default;

    // IForward
    IPlugin *getPlugin() const override;
    ExecType getExecType() const override;
    bool execFunc(std::int32_t *result) override;

private:
    /* plugin which the function will be executed in */
    IPlugin *m_plugin;
};

class ForwardMngr final : public IForwardMngr
{
public:
    ForwardMngr() = default;
    ForwardMngr(const ForwardMngr &other) = delete;
    ForwardMngr(ForwardMngr &&other) = delete;
    ~ForwardMngr() = default;
    ForwardMngr &operator=(const ForwardMngr &other) = delete;
    ForwardMngr &operator=(ForwardMngr &&other) = delete;

    // IForwardMngr
    Forward *createForward(std::string_view name,
                           Forward::ExecType exec = Forward::ExecType::Ignore,
                           std::array<IForward::IParam::Type, IForward::MAX_EXEC_PARAMS> params = {},
                           IPlugin *plugin = nullptr) override;
    void addForwardListener(Forward::Callback func) override;
    bool deleteForward(const IForward *forward) override;

    // ForwardMngr
    void clearForwards();

private:
    std::unordered_multimap<std::string, std::unique_ptr<Forward>> m_forwards;
    std::vector<Forward::Callback> m_callbacks;
};
