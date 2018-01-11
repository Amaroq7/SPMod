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

#include <IPluginSystem.hpp>

namespace SPMod
{
    class IForward
    {
    public:
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

        virtual const char *getName() const = 0;
        virtual IPlugin *getPlugin() const = 0;
        virtual bool pushCell(cell_t cell) = 0;
        virtual bool pushCellPtr(cell_t *cell, bool copyback) = 0;
        virtual bool pushFloat(float real) = 0;
        virtual bool pushFloatPtr(float *real, bool copyback) = 0;
        virtual bool pushArray(cell_t *array, size_t size, bool copyback) = 0;
        virtual bool pushString(const char *string) = 0;
        virtual bool pushStringEx(char *buffer, size_t length, StringFlags sflags, bool copyback) = 0;
        virtual bool execFunc(cell_t *result) = 0;
        virtual void resetParams() = 0;

    protected:
        virtual ~IForward() { }
    };

    class IForwardMngr
    {
    public:
        virtual IForward *createForward(const char *name,
                                        IForward::ExecType exec,
                                        size_t params,
                                        ...) = 0;

        virtual IForward *findForward(const char *name) = 0;

    protected:
        virtual ~IForwardMngr() { };
    };

    // ExecType operators
    inline uint8_t operator &(const IForward::ExecType lhs, const IForward::ExecType rhs)
    {
        return static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs);
    }

    inline IForward::ExecType operator &=(const IForward::ExecType lhs, const IForward::ExecType rhs)
    {
        auto returnVal = static_cast<uint8_t>(lhs);
        returnVal &= static_cast<uint8_t>(rhs);

        return static_cast<IForward::ExecType>(returnVal);
    }

    inline bool operator !(const IForward::ExecType lhs)
    {
        return !!static_cast<uint8_t>(lhs);
    }

    inline IForward::ExecType operator |(const IForward::ExecType lhs, const IForward::ExecType rhs)
    {
        return static_cast<IForward::ExecType>(static_cast<uint8_t>(lhs) |
                                                static_cast<uint8_t>(rhs));
    }

    inline IForward::ExecType operator |=(const IForward::ExecType lhs, const IForward::ExecType rhs)
    {
        auto returnVal = static_cast<uint8_t>(lhs);
        returnVal |= static_cast<uint8_t>(rhs);

        return static_cast<IForward::ExecType>(returnVal);
    }

    inline IForward::ExecType operator ^(const IForward::ExecType lhs, const IForward::ExecType rhs)
    {
        return static_cast<IForward::ExecType>(static_cast<uint8_t>(lhs) ^
                                                static_cast<uint8_t>(rhs));
    }

    inline IForward::ExecType operator ^=(const IForward::ExecType lhs, const IForward::ExecType rhs)
    {
        auto returnVal = static_cast<uint8_t>(lhs);
        returnVal ^= static_cast<uint8_t>(rhs);

        return static_cast<IForward::ExecType>(returnVal);
    }

    // StringFlags operators
    inline uint8_t operator &(const IForward::StringFlags lhs, const IForward::StringFlags rhs)
    {
        return static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs);
    }

    inline IForward::StringFlags operator &=(const IForward::StringFlags lhs, const IForward::StringFlags rhs)
    {
        auto returnVal = static_cast<uint8_t>(lhs);
        returnVal &= static_cast<uint8_t>(rhs);

        return static_cast<IForward::StringFlags>(returnVal);
    }

    inline bool operator !(const IForward::StringFlags lhs)
    {
        return !!static_cast<uint8_t>(lhs);
    }

    inline IForward::StringFlags operator |(const IForward::StringFlags lhs, const IForward::StringFlags rhs)
    {
        return static_cast<IForward::StringFlags>(static_cast<uint8_t>(lhs) |
                                                static_cast<uint8_t>(rhs));
    }

    inline IForward::StringFlags operator |=(const IForward::StringFlags lhs, const IForward::StringFlags rhs)
    {
        auto returnVal = static_cast<uint8_t>(lhs);
        returnVal |= static_cast<uint8_t>(rhs);

        return static_cast<IForward::StringFlags>(returnVal);
    }

    inline IForward::StringFlags operator ^(const IForward::StringFlags lhs, const IForward::StringFlags rhs)
    {
        return static_cast<IForward::StringFlags>(static_cast<uint8_t>(lhs) ^
                                                static_cast<uint8_t>(rhs));
    }

    inline IForward::StringFlags operator ^=(const IForward::StringFlags lhs, const IForward::StringFlags rhs)
    {
        auto returnVal = static_cast<uint8_t>(lhs);
        returnVal ^= static_cast<uint8_t>(rhs);

        return static_cast<IForward::StringFlags>(returnVal);
    }

    // ReturnValue operators
    inline bool operator ==(cell_t lhs, IForward::ReturnValue rhs)
    {
        return lhs == static_cast<uint8_t>(rhs);
    }

    inline bool operator !=(cell_t lhs, IForward::ReturnValue rhs)
    {
        return lhs != static_cast<uint8_t>(rhs);
    }
}
