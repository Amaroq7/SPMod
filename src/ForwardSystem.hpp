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

class Plugin;

/* @brief General forward used in SPMod.
 *        It can be either SingleForward or MutliForward.
 *        To distinguish them between them value returned from getPluginCore() needs to be checked against nullptr
 *        if it is nullptr then Forward is MultiForward, otherwise it is SingleForward.
 */
class Forward : public IForward
{
public:

    /* name of the forward */
    const char *getName() const override;

    /* id of the forward */
    std::size_t getId() const override;

    /* type of parameter */
    ParamType getParamType(std::size_t id) const override;

    /* number of parameters */
    std::size_t getParamsNum() const override;

    /* name of the forward for use across SPMod */
    std::string_view getNameCore() const;

    bool isExecuted() const;

    /* plugin which the function will be executed in */
    virtual std::shared_ptr<Plugin> getPluginCore() const = 0;

protected:

    /* forward name */
    std::string m_name;

    /* forward id */
    std::size_t m_id;

    /* parameters types */
    std::array<IForward::ParamType, SP_MAX_EXEC_PARAMS> m_paramTypes;

    /* number of already pushed params */
    std::size_t m_currentPos;

    /* number of parameters in forward */
    std::size_t m_paramsNum;

    /* true if forward is being executed */
    bool m_exec;
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
                 std::size_t id,
                 std::array<IForward::ParamType, SP_MAX_EXEC_PARAMS> paramstypes,
                 std::size_t params,
                 ExecType type);

    MultiForward() = delete;
    ~MultiForward() = default;

    // IForward
    IPlugin *getPlugin() const override;
    bool pushCell(cell_t cell) override;
    bool pushCellPtr(cell_t *cell,
                     bool copyback) override;

    bool pushFloat(float real) override;
    bool pushFloatPtr(float *real,
                      bool copyback) override;

    bool pushArray(cell_t *array,
                   std::size_t size,
                   bool copyback) override;

    bool pushString(const char *string) override;
    bool pushStringEx(char *buffer,
                      std::size_t length,
                      IForward::StringFlags sflags,
                      bool copyback) override;

    bool execFunc(cell_t *result) override;
    void resetParams() override;

    // Forward
    std::shared_ptr<Plugin> getPluginCore() const override;

private:

    /* helper function to push params to plugin function */
    void pushParamsToFunction(SourcePawn::IPluginFunction *func);

    /* defines how parameters are stored in cache */
    struct ForwardParam
    {
        std::variant<cell_t, cell_t *, float, float *, const char *, char *> m_param;
        bool m_copyback;
        std::size_t m_size;
        IForward::StringFlags m_stringFlags;
    };

    /* exec type of forward */
    ExecType m_execType;

    /* stores cached parameters */
    std::array<ForwardParam, SP_MAX_EXEC_PARAMS> m_params;
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
                  std::size_t id,
                  std::array<IForward::ParamType, SP_MAX_EXEC_PARAMS> paramstypes,
                  std::size_t params,
                  std::shared_ptr<Plugin> plugin);

    SingleForward() = delete;
    ~SingleForward() = default;

    // IForward
    IPlugin *getPlugin() const override;
    bool pushCell(cell_t cell) override;
    bool pushCellPtr(cell_t *cell,
                     bool copyback) override;

    bool pushFloat(float real) override;
    bool pushFloatPtr(float *real,
                      bool copyback) override;

    bool pushArray(cell_t *array,
                   std::size_t size,
                   bool copyback) override;

    bool pushString(const char *string) override;
    bool pushStringEx(char *buffer,
                      std::size_t length,
                      IForward::StringFlags sflags,
                      bool copyback) override;

    bool execFunc(cell_t *result) override;
    void resetParams() override;

    // Forward
    std::shared_ptr<Plugin> getPluginCore() const override;

private:

    /* plugin which the function will be executed in */
    std::weak_ptr<Plugin> m_plugin;

    /* plugin function which will be executed */
    SourcePawn::IPluginFunction *m_pluginFunc;
};


class ForwardMngr final : public IForwardMngr
{
public:

    enum class FwdDefault : uint8_t
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
    ~ForwardMngr() = default;

    // IForwardMngr
    IForward *createForward(const char *name,
                            IForward::ExecType exec,
                            std::size_t params,
                            ...) override;

    IForward *createForward(const char *name,
                            IPlugin *plugin,
                            std::size_t params,
                            ...) override;

    void deleteForward(IForward *forward) override;
    std::size_t getForwardsNum() const override;
    const ForwardList *getForwardsList() const override;
    void freeForwardsList(const ForwardList *list) const override;

    // ForwardMngr
    void clearForwards();
    void deleteForwardCore(std::shared_ptr<Forward> fwd);
    std::shared_ptr<Forward> getDefaultForward(ForwardMngr::FwdDefault fwd) const;

    void addDefaultsForwards();

    std::shared_ptr<Forward> createForwardCore(std::string_view name,
                                               IForward::ExecType exec,
                                               std::array<IForward::ParamType, SP_MAX_EXEC_PARAMS> params,
                                               std::size_t paramsnum,
                                               std::shared_ptr<Plugin> plugin = nullptr);

    std::shared_ptr<Forward> findForward(std::size_t id) const;

private:
    std::shared_ptr<Forward> _createForwardVa(std::string_view name,
                                              IForward::ExecType exec,
                                              std::va_list params,
                                              std::size_t paramsnum,
                                              IPlugin *plugin = nullptr);

    std::unordered_map<std::string, std::shared_ptr<Forward>> m_forwards;

    /* keeps track of forwards ids */ 
    std::size_t m_id;

    /* cache for defaults forwards */
    std::array<std::weak_ptr<Forward>, defaultForwardsNum> m_defaultForwards;
};
