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
    Forward(const std::string &name,
            std::array<ParamType, SP_MAX_EXEC_PARAMS> paramstypes,
            size_t params,
            ExecType type) : m_name(name),
                                m_execType(type),
                                m_paramTypes(paramstypes),
                                m_plugin(nullptr),
                                m_currentPos(0),
                                m_paramsNum(params)
                                { }
    Forward(const std::string &name,
            std::array<ParamType, SP_MAX_EXEC_PARAMS> paramstypes,
            size_t params,
            const Plugin *plugin) : m_name(name),
                                    m_execType(ExecType::HIGHEST),
                                    m_paramTypes(paramstypes),
                                    m_plugin(plugin),
                                    m_currentPos(0),
                                    m_paramsNum(params)
                                    { }
    ~Forward() = default;

    // IForward
    const char *getName() const override
    {
        return m_name.c_str();
    }
    const IPlugin *getPlugin() const override
    {
        return reinterpret_cast<const IPlugin *>(m_plugin);
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
                        StringFlags sflags,
                        bool copyback) override;
    bool execFunc(cell_t *result) override;
    void resetParams() override;

    const std::string &getNameString() const
    {
        return m_name;
    }

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
    std::array<ParamType, SP_MAX_EXEC_PARAMS> m_paramTypes;
    std::array<ForwardParam, SP_MAX_EXEC_PARAMS> m_params;
    const Plugin *m_plugin;
    size_t m_currentPos;
    size_t m_paramsNum;
};

class ForwardMngr final : public IForwardMngr
{
public:
    ForwardMngr() = default;
    ~ForwardMngr() = default;

    // IForwardMngr
    IForward *createForward(const char *name,
                            IForward::ExecType exec,
                            size_t params,
                            ...) override;

    IForward *findForward(const char *name) override;

    // ForwardMngr
    Forward *createForward(const std::string &name,
                            Forward::ExecType exec,
                            const std::initializer_list<Forward::ParamType> &params);

    bool addForward(std::shared_ptr<Forward> forward)
    {
        return m_forwards.insert(std::make_pair(forward->getNameString(),
                                                forward)).second;
    }

    void clearForwards()
    {
        m_forwards.clear();
    }

private:
    std::unordered_map<std::string, std::shared_ptr<Forward>> m_forwards;
};
