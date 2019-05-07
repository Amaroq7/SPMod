/*
 *  Copyright (C) 2018 SPMod Development Team
 *
 *  This file is part of SPMod.
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

#pragma once

#include "spmod.hpp"

/* @brief General forward used in SPMod.
 *        It can be either SingleForward or MutliForward.
 *        To distinguish them between them value returned from getPluginCore() needs to be checked against nullptr
 *        if it is nullptr then Forward is MultiForward, otherwise it is SingleForward.
 */
class Forward : public IForward
{
public:
    Forward(std::string_view name,
            std::array<IForward::Param::Type, IForward::MAX_EXEC_PARAMS> paramstypes,
            std::size_t params,
            const std::vector<ForwardCallback> &callbacks);

    /* name of the forward */
    const char *getName() const override;

    /* number of parameters */
    std::size_t getParamsNum() const override;

    /* param of the forward */
    const IForward::Param *getParam(std::size_t id) const override;

    /* name of the forward for use across SPMod */
    std::string_view getNameCore() const;

    bool pushInt(int integer) override;
    bool pushInt(int *integer,
                 bool copyback) override;

    bool pushFloat(float real) override;
    bool pushFloat(float *real,
                   bool copyback) override;

    bool pushArray(void *array,
                   std::size_t size,
                   bool copyback) override;

    bool pushString(const char *string) override;
    bool pushString(char *buffer,
                    std::size_t length,
                    IForward::StringFlags sflags,
                    bool copyback) override;

    bool isExecuted() const;

    void resetParams() override;

protected:
    /* forward name */
    std::string m_name;

    /* number of parameters in forward */
    std::size_t m_paramsNum;

    /* true if forward is being executed */
    bool m_exec;

    /* stores parameters */
    std::array<IForward::Param, MAX_EXEC_PARAMS> m_params;

    /* extensions' callbacks */
    const std::vector<ForwardCallback> &m_callbacks;

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
                 std::array<IForward::Param::Type, MAX_EXEC_PARAMS> paramstypes,
                 std::size_t params,
                 ExecType type,
                 const std::vector<ForwardCallback> &callbacks);

    MultiForward() = delete;
    MultiForward(const MultiForward &other) = delete;
    MultiForward(MultiForward &&other) = default;
    ~MultiForward() = default;

    // IForward
    IPlugin *getPlugin() const override;
    ExecType getExecType() const override;

    bool execFunc(int *result) override;

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
                  std::array<IForward::Param::Type, MAX_EXEC_PARAMS> paramstypes,
                  std::size_t params,
                  IPlugin *plugin,
                  const std::vector<ForwardCallback> &callbacks);

    SingleForward() = delete;
    SingleForward(const SingleForward &other) = delete;
    SingleForward(SingleForward &&other) = default;
    ~SingleForward() = default;

    // IForward
    IPlugin *getPlugin() const override;
    ExecType getExecType() const override;
    bool execFunc(int *result) override;

private:
    /* plugin which the function will be executed in */
    IPlugin *m_plugin;
};

class ForwardMngr final : public IForwardMngr
{
public:

    enum class FwdDefault : std::uint8_t
    {
        ClientConnect = 0,
        ClientDisconnect,
        ClientPutInServer,
        PluginsLoaded,
        PluginInit,
        PluginEnd,
        PluginNatives,
        ClientCommmand,
        MapChange,

        /* number of defaults forwards */
        ForwardsNum,
    };
    static constexpr std::size_t defaultForwardsNum = static_cast<std::size_t>(FwdDefault::ForwardsNum);

    ForwardMngr() = default;
    ForwardMngr(const ForwardMngr &other) = delete;
    ForwardMngr(ForwardMngr &&other) = default;
    ~ForwardMngr() = default;

    // IForwardMngr
    IForward *createForward(const char *name,
                            IForward::ExecType exec,
                            std::size_t paramsnum,
                            IForward::Param::Type *params) override;

    IForward *createForward(const char *name,
                            IPlugin *plugin,
                            std::size_t paramsnum,
                            IForward::Param::Type *params) override;

    void deleteForward(IForward *forward) override;
    void addForwardListener(ForwardCallback func) override;

    // ForwardMngr
    void clearForwards();
    void deleteForwardCore(std::shared_ptr<Forward> fwd);
    std::shared_ptr<Forward> getDefaultForward(ForwardMngr::FwdDefault fwd) const;

    void addDefaultsForwards();

    std::shared_ptr<Forward> createForwardCore(std::string_view name,
                                               IForward::ExecType exec,
                                               std::array<IForward::Param::Type, IForward::MAX_EXEC_PARAMS> params,
                                               std::size_t paramsnum,
                                               IPlugin *plugin = nullptr);

private:
    std::shared_ptr<Forward> _createForward(std::string_view name,
                                            IForward::ExecType exec,
                                            IForward::Param::Type *params,
                                            std::size_t paramsnum,
                                            IPlugin *plugin = nullptr);

    std::unordered_map<std::string, std::shared_ptr<Forward>> m_forwards;
    std::vector<ForwardCallback> m_callbacks;

    /* cache for defaults forwards */
    std::array<std::weak_ptr<Forward>, defaultForwardsNum> m_defaultForwards;
};
