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
    class IForward SPMOD_FINAL
    {
    public:

        /*
         * Execution types
         */
        enum class ExecType : uint8_t
        {
            /* Ignore returned result */
            Ignore = 0,

            /* Stop executing when plugin return ReturnValue::PluginStop */
            Stop = (1 << 0),

            /* Return the highest value returned by plugins */
            Highest = (1 << 1)
        };

        /*
         * Param types
         */
        enum class ParamType : uint8_t
        {
            None = 0,
            Cell,
            CellRef,
            Float,
            FloatRef,
            Array,
            String
        };

        /*
         * Predefined values that can be returned by plugins
         */
        enum class ReturnValue : uint8_t
        {
            PluginIgnored = 0,
            PluginStop,
            PluginHandled
        };

        /*
         * String flags for StringEx param
         */
        enum class StringFlags : uint8_t
        {
            /* No special behavior */
            None = 0,

            /* String should be UTF-8 handled */
            Utf8 = (1 << 0),

            /* String should be copied into the plugin */
            Copy = (1 << 1),

            /* String should be handled as binary data */
            Binary = (1 << 2)
        };

        /*
         * @brief Returns forward name.
         *
         * @note  Name of the forward is also the name of function
         *        that will be executed in a plugin.
         *
         * @return      Null-terminated name.
         */
        virtual const char *getName() const = 0;

        /*
         * @brief Returns plugin which the forward will be executed in.
         *
         * @return      Plugin pointer, nullptr if forward will be executed in all plugins.
         */
        virtual IPlugin *getPlugin() const = 0;

        /*
         * @brief Returns param type.
         *
         * @param id    Param id.
         *
         * @return      Param type.
         */
        virtual ParamType getParamType(size_t id) const = 0;

        /*
         * @brief Returns params num.
         *
         * @return      Param num.
         */
        virtual size_t getParamsNum() const = 0;

        /*
         * @brief Returns forward id.
         *
         * @return      Forward id.
         */
        virtual size_t getId() const = 0;

        /*
         * @brief Pushes cell to current call.
         *
         * @param cell    Cell to pass.
         *
         * @return        True if succeed, false if parameter type is wrong.
         */
        virtual bool pushCell(cell_t cell) = 0;

        /*
         * @brief Pushes cell pointer to current call.
         *
         * @param cell      Cell pointer to pass.
         * @param copyback  True if copy back value, false to not.
         *
         * @return          True if succeed, false if parameter type is wrong.
         */
        virtual bool pushCellPtr(cell_t *cell,
                                 bool copyback) = 0;

        /*
         * @brief Pushes real to current call.
         *
         * @param real    Real to pass.
         *
         * @return        True if succeed, false if parameter type is wrong.
         */
        virtual bool pushFloat(float real) = 0;

        /*
         * @brief Pushes real pointer to current call.
         *
         * @param real      Real pointer to pass.
         * @param copyback  True if copy back value, false to not.
         *
         * @return          True if succeed, false if parameter type is wrong.
         */
        virtual bool pushFloatPtr(float *real,
                                  bool copyback) = 0;

        /*
         * @brief Pushes array to current call.
         *
         * @param array     Array to pass.
         * @param size      Size of the array.
         * @param copyback  True if copy back value, false to not.
         *
         * @return          True if succeed, false if parameter type is wrong.
         */
        virtual bool pushArray(cell_t *array,
                                size_t size,
                                bool copyback) = 0;

        /*
         * @brief Pushes string to current call.
         *
         * @param string    String to pass.
         *
         * @return          True if succeed, false if parameter type is wrong.
         */
        virtual bool pushString(const char *string) = 0;

        /*
         * @brief Pushes string to current call.
         *
         * @param string    String to pass.
         * @param length    Length of the string.
         * @param sflags    String flags.
         * @param copyback  True if copy back value, false to not.
         *
         * @return          True if succeed, false if parameter type is wrong.
         */
        virtual bool pushStringEx(char *buffer,
                                  size_t length,
                                  StringFlags sflags,
                                  bool copyback) = 0;

        /*
         * @brief Pushes string to current call.
         *
         * @note If execution has been successful, then pushed params are reset.
         *
         * @note Param result can be nullptr only if exec type of forward is ignore.
         *
         * @param result    Address where the result will be stored.
         *
         * @return          True if succeed, false if execution failed.
         */
        virtual bool execFunc(cell_t *result) = 0;

        /*
         * @brief Resets params already pushed to forward.
         *
         * @noreturn
         */
        virtual void resetParams() = 0;

    protected:
        virtual ~IForward() {}
    };

    struct ForwardList
    {
        /* Current forward */
        IForward *forward;

        /* Next forward in the list, nullptr if there is no more forwards */
        ForwardList *next;
    };

    class IForwardMngr SPMOD_FINAL
    {
    public:

        /*
         * @brief Creates forward.
         *
         * @note  Name of the forward is also the name of function
         *        that will be executed in a plugin.
         *
         * @param name      Name of the forward.
         * @param exec      Exec type.
         * @param params    Number of parameters, cannot exceed 32.
         * @param ...       Types of parameters (IForward::ParamType).
         *
         * @return          Forward pointer, nullptr if failed.
         */
        virtual IForward *createForward(const char *name,
                                        IForward::ExecType exec,
                                        size_t params,
                                        ...) = 0;

        /*
         * @brief Creates forward.
         *
         * @note  Name of the forward is also the name of function
         *        that will be executed in a plugin.
         *
         * @param name      Name of the forward.
         * @param plugin    Plugin which the forward will be executed in.
         * @param params    Number of parameters, cannot exceed 32.
         * @param ...       Types of parameters (IForward::ParamType).
         *
         * @return          Forward pointer, nullptr if failed.
         */
        virtual IForward *createForward(const char *name,
                                        IPlugin *plugin,
                                        size_t params,
                                        ...) = 0;

        /*
         * @brief Deletes forward.
         *
         * @param forward   Forward to remove.
         *
         * @noreturn
         */
        virtual void deleteForward(IForward *forward) = 0;

        /*
         * @brief Returns number of forwards.
         *
         * @return          Number of created forwards.
         */
        virtual size_t getForwardsNum() const = 0;

        /*
         * @brief Gets list of forwards.
         *
         * @note Static list. It is safe to remove forwards while iterating it.
         * @note Needs to be freed using freeForwardsList().
         *
         * @return          Pointer to list, nullptr if there is no forwards.
         */
         virtual const ForwardList *getForwardsList() const = 0;

        /*
         * @brief Frees list of forwards.
         *
         * @noreturn
         */
         virtual void freeForwardsList(const ForwardList *list) const = 0;

    protected:
        virtual ~IForwardMngr() {}
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
