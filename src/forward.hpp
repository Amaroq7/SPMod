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

class Forward// final : public IForward
{
public:
    // TODO: Move these enums to interface header and operators overloads as well
    enum class ExecType : uint8_t
    {
        IGNORE = 0,
        STOP = (1 << 0),
        HIGHEST = (1 << 1)
    };
    enum class ParamType : uint8_t
    {
        CELL = 0,
        CELL_REF,
        FLOAT,
        FLOAT_REF,
        ARRAY,
        STRING,
        STRINGEX
    };
    enum class ReturnValue : uint8_t
    {
        PLUGIN_IGNORED = 0,
        PLUGIN_STOP,
    };
    enum class StringFlags : uint8_t
    {
        NONE = 0,
        UTF8 = (1 << 0),
        COPY = (1 << 1),
        BINARY = (1 << 2)
    };

    Forward(const std::string &name,
            std::array<ParamType, SP_MAX_EXEC_PARAMS> &&paramstypes,
            ExecType type,
            Plugin *plugin = nullptr) : m_name(name), m_execType(type),
                                        m_paramTypes(paramstypes), m_plugin(plugin),
                                        m_currentPos(0)
                                        { }
    ~Forward() = default;

    // IForward
    const char *getName() const
    {
        return m_name.c_str();
    }
    IPlugin *getPlugin() const
    {
        return m_plugin;
    }
    bool pushCell(cell_t cell);
    bool pushCellPtr(cell_t *cell, bool copyback);
    bool pushFloat(float real);
    bool pushFloatPtr(float *real, bool copyback);
    bool pushArray(cell_t *array, size_t size, bool copyback);
    bool pushString(const char *string);
    bool pushStringEx(char *buffer, size_t length, StringFlags sflags, bool copyback);
    bool execFunc(cell_t *result);
    bool resetParams();

    const std::string &getNameString() const
    {
        return m_name;
    }

private:

    void pushParamsToFunction(SourcePawn::IPluginFunction *func);

    struct ForwardParam
    {
        std::variant<cell_t, cell_t *, float, float *, const char *, char *> m_param;
        bool m_copyback;
        size_t m_size;
        StringFlags m_stringFlags;
    };

    std::string m_name;
    ExecType m_execType;
    std::array<ParamType, SP_MAX_EXEC_PARAMS> m_paramTypes;
    std::array<ForwardParam, SP_MAX_EXEC_PARAMS> m_params;
    Plugin *m_plugin;
    size_t m_currentPos;
    size_t m_paramsNum;
};

bool operator &(const Forward::ExecType lhs, const Forward::ExecType rhs)
{
    return !!(static_cast<std::underlying_type<Forward::ExecType>::type>(lhs) &
            static_cast<std::underlying_type<Forward::ExecType>::type>(rhs));
}

bool operator !(const Forward::ExecType lhs)
{
    return !!static_cast<std::underlying_type<Forward::ExecType>::type>(lhs);
}

bool operator ==(int lhs, Forward::ReturnValue rhs)
{
    return lhs == static_cast<std::underlying_type<Forward::ReturnValue>::type>(rhs);
}

bool operator &(const Forward::StringFlags lhs, const Forward::StringFlags rhs)
{
    return !!(static_cast<std::underlying_type<Forward::StringFlags>::type>(lhs) &
            static_cast<std::underlying_type<Forward::StringFlags>::type>(rhs));
}
