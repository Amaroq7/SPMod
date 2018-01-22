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

#pragma once

#include "spmod.hpp"

class Plugin;

class Forward final : public IForward
{
public:
    Forward(std::string_view name,
            fwdOwnerVariant owner,
            fwdParamTypeList paramstypes,
            size_t params,
            ExecType type) : m_name(name),
                                m_execType(type),
                                m_paramTypes(paramstypes),
                                m_plugin(std::shared_ptr<Plugin>(nullptr)),
                                m_currentPos(0),
                                m_paramsNum(params),
                                m_owner(owner)
                                { }
    Forward(std::string_view name,
            fwdOwnerVariant owner,
            fwdParamTypeList paramstypes,
            size_t params,
            std::shared_ptr<Plugin> plugin) : m_name(name),
                                                m_execType(ExecType::HIGHEST),
                                                m_paramTypes(paramstypes),
                                                m_plugin(plugin),
                                                m_currentPos(0),
                                                m_paramsNum(params),
                                                m_owner(owner)
                                                { }
    Forward() = delete;
    ~Forward() = default;

    // IForward
    const char *getName() const override
    {
        return m_name.c_str();
    }
    IPlugin *getPlugin() const override
    {
        return reinterpret_cast<IPlugin *>(m_plugin.lock().get());
    }
    ParamType getParamType(size_t id) const override
    {
        return m_paramTypes.at(id);
    }
    size_t getParamsNum() const override
    {
        return m_paramsNum;
    }
    IPlugin *getOwnerPlugin() const override;
    IModuleInterface *getOwnerModule() const override;
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
                        StringFlags sflags,
                        bool copyback) override;
    bool execFunc(cell_t *result) override;
    void resetParams() override;

    // Forward
    const std::string &getNameString() const
    {
        return m_name;
    }
    std::shared_ptr<Plugin> getOwnerPluginCore() const;

private:
    void pushParamsToFunction(SourcePawn::IPluginFunction *func);

    struct ForwardParam
    {
        std::variant<cell_t,
                    cell_t *,
                    float,
                    float *,
                    const char *,
                    char *> m_param;
        bool m_copyback;
        size_t m_size;
        StringFlags m_stringFlags;
    };

    std::string m_name;
    ExecType m_execType;
    fwdParamTypeList m_paramTypes;
    std::array<ForwardParam, SP_MAX_EXEC_PARAMS> m_params;
    std::weak_ptr<Plugin> m_plugin;
    size_t m_currentPos;
    size_t m_paramsNum;

    fwdOwnerVariant m_owner;
};

class ForwardMngr final : public IForwardMngr
{
public:
    ForwardMngr()
    {
        _addDefaultsForwards();
    }
    ~ForwardMngr() = default;

    // IForwardMngr
    IForward *createForward(const char *name,
                            IModuleInterface *owner,
                            IForward::ExecType exec,
                            size_t params,
                            ...) override;

    IForward *createForward(const char *name,
                            IPlugin *owner,
                            IForward::ExecType exec,
                            size_t params,
                            ...) override;

    IForward *findForward(const char *name) const override;

    // ForwardMngr
    bool addForward(std::shared_ptr<Forward> forward)
    {
        return m_forwards.try_emplace(forward->getNameString(), forward).second;
    }
    void clearForwards()
    {
        m_forwards.clear();
    }
    void clearNonDefaults();
    std::shared_ptr<Forward> createForwardCore(std::string_view name,
                                                fwdOwnerVariant owner,
                                                IForward::ExecType exec,
                                                fwdInitParamsList params);

    std::shared_ptr<Forward> findForwardCore(std::string_view name) const;

    void deletePluginForwards(std::string_view identity);

private:
    void _addDefaultsForwards();

    std::shared_ptr<Forward> _createForwardVa(std::string_view name,
                                                fwdOwnerVariant owner,
                                                IForward::ExecType exec,
                                                va_list params,
                                                size_t paramsnum);

    std::shared_ptr<Forward> _createForward(std::string_view name,
                                            fwdOwnerVariant owner,
                                            IForward::ExecType exec,
                                            fwdParamTypeList params,
                                            size_t paramsnum);

    std::unordered_map<std::string, std::shared_ptr<Forward>> m_forwards;
};
