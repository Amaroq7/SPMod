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
    const char *getName() const override
    {
        return m_name.c_str();
    }

    /* id of the forward */
    size_t getId() const override
    {
        return m_id;
    }

    /* type of parameter */
    ParamType getParamType(size_t id) const override
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

    /* number of parameters */
    size_t getParamsNum() const override
    {
        return m_paramsNum;
    }

    /* name of the forward for use across SPMod */
    std::string_view getNameCore() const
    {
        return m_name;
    }
    bool isExecuted() const
    {
        return m_exec;
    }

    /* plugin which the function will be executed in */
    virtual std::shared_ptr<Plugin> getPluginCore() const = 0;

protected:

    /* forward name */
    std::string m_name;

    /* forward id */
    size_t m_id;

    /* parameters types */
    std::array<IForward::ParamType, SP_MAX_EXEC_PARAMS> m_paramTypes;

    /* number of already pushed params */
    size_t m_currentPos;

    /* number of parameters in forward */
    size_t m_paramsNum;

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
                 size_t id,
                 std::array<IForward::ParamType, SP_MAX_EXEC_PARAMS> paramstypes,
                 size_t params,
                 ExecType type);

    MultiForward() = delete;
    ~MultiForward() = default;

    // IForward
    IPlugin *getPlugin() const override
    {
        return nullptr;
    }
    bool pushCell(cell_t cell) override;
    bool pushCellPtr(cell_t *cell,
                     bool copyback) override;

    bool pushFloat(float real) override;
    bool pushFloatPtr(float *real,
                      bool copyback) override;

    bool pushArray(cell_t *array,
                   size_t size,
                   bool copyback) override;

    bool pushString(const char *string) override;
    bool pushStringEx(char *buffer,
                      size_t length,
                      IForward::StringFlags sflags,
                      bool copyback) override;
    bool execFunc(cell_t *result) override;
    void resetParams() override
    {
        m_currentPos = 0;
    }

    // Forward
    std::shared_ptr<Plugin> getPluginCore() const override
    {
        return nullptr;
    }

private:

    /* helper function to push params to plugin function */
    void pushParamsToFunction(SourcePawn::IPluginFunction *func);

    /* defines how parameters are stored in cache */
    struct ForwardParam
    {
        std::variant<cell_t, cell_t *, float, float *, const char *, char *> m_param;
        bool m_copyback;
        size_t m_size;
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
                  size_t id,
                  std::array<IForward::ParamType, SP_MAX_EXEC_PARAMS> paramstypes,
                  size_t params,
                  std::shared_ptr<Plugin> plugin);

    SingleForward() = delete;
    ~SingleForward() = default;

    // IForward
    IPlugin *getPlugin() const override
    {
        return reinterpret_cast<IPlugin *>(m_plugin.lock().get());
    }
    bool pushCell(cell_t cell) override;
    bool pushCellPtr(cell_t *cell,
                     bool copyback) override;

    bool pushFloat(float real) override;
    bool pushFloatPtr(float *real,
                      bool copyback) override;

    bool pushArray(cell_t *array,
                   size_t size,
                   bool copyback) override;

    bool pushString(const char *string) override;
    bool pushStringEx(char *buffer,
                      size_t length,
                      IForward::StringFlags sflags,
                      bool copyback) override;
    bool execFunc(cell_t *result) override;
    void resetParams() override
    {
        m_pluginFunc->Cancel();
    }

    // Forward
    std::shared_ptr<Plugin> getPluginCore() const override
    {
        return m_plugin.lock();
    }

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
    static constexpr size_t defaultForwardsNum = static_cast<size_t>(FwdDefault::ForwardsNum);

    ForwardMngr() = default;
    ~ForwardMngr() = default;

    // IForwardMngr
    IForward *createForward(const char *name,
                            IForward::ExecType exec,
                            size_t params,
                            ...) override;

    IForward *createForward(const char *name,
                            IPlugin *plugin,
                            size_t params,
                            ...) override;

    IForward *findForward(const char *name) const override;
    IForward *findForward(size_t id) const override;
    void deleteForward(IForward *forward) override
    {
        m_forwards.erase(forward->getName());
    }
    size_t getForwardsNum() const override
    {
        return m_forwards.size();
    }
    const ForwardList *getForwardsList() const override;
    void freeForwardsList(const ForwardList *list) const override;

    // ForwardMngr
    void clearForwards()
    {
        m_forwards.clear();
        m_id = 0;
    }
    void deleteForwardCore(std::shared_ptr<Forward> fwd)
    {
        m_forwards.erase(fwd->getNameCore().data());
    }
    std::shared_ptr<Forward> getDefaultForward(ForwardMngr::FwdDefault fwd) const
    {
        return m_defaultForwards.at(static_cast<size_t>(fwd)).lock();
    }

    void addDefaultsForwards();

    std::shared_ptr<Forward> createForwardCore(std::string_view name,
                                               IForward::ExecType exec,
                                               std::array<IForward::ParamType, SP_MAX_EXEC_PARAMS> params,
                                               size_t paramsnum,
                                               std::shared_ptr<Plugin> plugin = nullptr);

    std::shared_ptr<Forward> findForwardCore(std::string_view name) const;
    std::shared_ptr<Forward> findForwardCore(size_t id) const;

private:
    std::shared_ptr<Forward> _createForwardVa(std::string_view name,
                                              IForward::ExecType exec,
                                              std::va_list params,
                                              size_t paramsnum,
                                              IPlugin *plugin = nullptr);

    std::unordered_map<std::string, std::shared_ptr<Forward>> m_forwards;

    /* keeps track of forwards ids */ 
    size_t m_id;

    /* cache for defaults forwards */
    std::array<std::weak_ptr<Forward>, defaultForwardsNum> m_defaultForwards;
};
