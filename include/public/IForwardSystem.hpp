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

#include <type_traits>

namespace SPMod
{
    class IPlugin;
    class IModuleInterface;
    class IForward SPMOD_FINAL
    {
    public:
        enum class ExecType : uint8_t
        {
            Ignore = 0,
            Stop = (1 << 0),
            Highest = (1 << 1)
        };
        enum class ParamType : uint8_t
        {
            Cell = 0,
            CellRef,
            Float,
            FloatRef,
            Array,
            String,
            StringEx
        };
        enum class ReturnValue : uint8_t
        {
            PluginIgnored = 0,
            PluginStop,
        };
        enum class StringFlags : uint8_t
        {
            None = 0,
            Utf8 = (1 << 0),
            Copy = (1 << 1),
            Binary = (1 << 2)
        };

        virtual const char *getName() const = 0;
        virtual IPlugin *getPlugin() const = 0;
        virtual ParamType getParamType(size_t id) const = 0;
        virtual size_t getParamsNum() const = 0;
        virtual IPlugin *getOwnerPlugin() const = 0;
        virtual IModuleInterface *getOwnerModule() const = 0;
        virtual bool pushCell(cell_t cell) = 0;
        virtual bool pushCellPtr(cell_t *cell,
                                    bool copyback) = 0;
        virtual bool pushFloat(float real) = 0;
        virtual bool pushFloatPtr(float *real,
                                    bool copyback) = 0;
        virtual bool pushArray(cell_t *array,
                                size_t size,
                                bool copyback) = 0;
        virtual bool pushString(const char *string) = 0;
        virtual bool pushStringEx(char *buffer,
                                    size_t length,
                                    StringFlags sflags,
                                    bool copyback) = 0;
        virtual bool execFunc(cell_t *result) = 0;
        virtual void resetParams() = 0;

    protected:
        virtual ~IForward() { }
    };

    class IForwardMngr SPMOD_FINAL
    {
    public:
        virtual IForward *createForward(const char *name,
                                        IModuleInterface *owner,
                                        IForward::ExecType exec,
                                        size_t params,
                                        ...) = 0;

        virtual IForward *createForward(const char *name,
                                        IPlugin *owner,
                                        IForward::ExecType exec,
                                        size_t params,
                                        ...) = 0;

        virtual IForward *findForward(const char *name) const = 0;


    protected:
        virtual ~IForwardMngr() { };
    };

    template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    inline bool hasEnumFlag(const T type,
                            const T flag)
    {
        return static_cast<bool>(static_cast<std::underlying_type_t<T>>(type) &
                                static_cast<std::underlying_type_t<T>>(flag));
    }

    template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    inline T operator &(const T lhs,
                        const T rhs)
    {
        return static_cast<T>(static_cast<std::underlying_type_t<T>>(lhs) &
                                static_cast<std::underlying_type_t<T>>(rhs));
    }

    template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    inline T operator &=(const T lhs,
                        const T rhs)
    {
        auto returnVal = static_cast<std::underlying_type_t<T>>(lhs);
        returnVal &= static_cast<std::underlying_type_t<T>>(rhs);

        return static_cast<T>(returnVal);
    }

    template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    inline T operator |(const T lhs,
                        const T rhs)
    {
        return static_cast<T>(static_cast<std::underlying_type_t<T>>(lhs) |
                                static_cast<std::underlying_type_t<T>>(rhs));
    }

    template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    inline T operator |=(const T lhs,
                        const T rhs)
    {
        auto returnVal = static_cast<std::underlying_type_t<T>>(lhs);
        returnVal |= static_cast<std::underlying_type_t<T>>(rhs);

        return static_cast<T>(returnVal);
    }

    template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    inline T operator ^(const T lhs,
                        const T rhs)
    {
        return static_cast<T>(static_cast<std::underlying_type_t<T>>(lhs) ^
                                static_cast<std::underlying_type_t<T>>(rhs));
    }

    template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    inline T operator ^=(const T lhs,
                        const T rhs)
    {
        auto returnVal = static_cast<std::underlying_type_t<T>>(lhs);
        returnVal ^= static_cast<std::underlying_type_t<T>>(rhs);

        return static_cast<T>(returnVal);
    }

    template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    inline bool operator ==(cell_t lhs,
                            const T rhs)
    {
        return lhs == static_cast<std::underlying_type_t<T>>(rhs);
    }

    template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    inline bool operator !=(cell_t lhs,
                            const T rhs)
    {
        return lhs != static_cast<std::underlying_type_t<T>>(rhs);
    }
}
