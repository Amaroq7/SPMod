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
            Array = (1 << 0),
            Int = (1 << 1),
            IntRef = (1 << 2),
            Float = (1 << 3),
            FloatRef = (1 << 4),
            String = (1 << 5),
            Data = (1 << 6)    // Generic data
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

        struct Param
        {
            void *m_data;
            std::size_t m_size; /* Array or stringEx length */
            bool m_copyback; /* True if data is meant to be overwritten */
            StringFlags m_stringFlags; /* String flags */
        };

        static constexpr std::size_t MAX_EXEC_PARAMS = 32;

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
        virtual ParamType getParamType(std::size_t id) const = 0;

        /*
         * @brief Returns params num.
         *
         * @return      Param num.
         */
        virtual size_t getParamsNum() const = 0;

        /*
         * @brief Pushes int to the current call.
         *
         * @param int     Integer to pass.
         *
         * @return        True if succeed, false if parameter type is wrong.
         */
        virtual bool pushInt(int integer) = 0;

        /*
         * @brief Pushes integer pointer to the current call.
         *
         * @param integer   Integer pointer to pass.
         * @param copyback  True if copy back value, false to not.
         *
         * @return          True if succeed, false if parameter type is wrong.
         */
        virtual bool pushIntPtr(int *integer,
                                bool copyback) = 0;

        /*
         * @brief Pushes real to the current call.
         *
         * @param real    Real to pass.
         *
         * @return        True if succeed, false if parameter type is wrong.
         */
        virtual bool pushFloat(float real) = 0;

        /*
         * @brief Pushes real pointer to the current call.
         *
         * @param real      Real pointer to pass.
         * @param copyback  True if copy back value, false to not.
         *
         * @return          True if succeed, false if parameter type is wrong.
         */
        virtual bool pushFloatPtr(float *real,
                                  bool copyback) = 0;

        /*
         * @brief Pushes array to the current call.
         *
         * @param array     Array to pass.
         * @param size      Size of the array.
         * @param copyback  True if copy back value, false to not.
         *
         * @return          True if succeed, false if parameter type is wrong.
         */
        virtual bool pushArray(void *array,
                               std::size_t size,
                               bool copyback) = 0;

        /*
         * @brief Pushes string to the current call.
         *
         * @param string    String to pass.
         *
         * @return          True if succeed, false if parameter type is wrong.
         */
        virtual bool pushString(const char *string) = 0;

        /*
         * @brief Pushes string to the current call.
         *
         * @param string    String to pass.
         * @param length    Length of the string.
         * @param sflags    String flags.
         * @param copyback  True if copy back value, false to not.
         *
         * @return          True if succeed, false if parameter type is wrong.
         */
        virtual bool pushStringEx(char *buffer,
                                  std::size_t length,
                                  StringFlags sflags,
                                  bool copyback) = 0;

        /*
         * @brief Pushes generic data to the current call.
         *
         * @param data      Generic data to pass.
         * @param copyback  True if copy back value, false to not.
         *
         * @return          True if succeed, false if parameter type is wrong.
         */
        virtual bool pushData(void *data,
                              bool copyback) = 0;

        /*
         * @brief Pushes string to the current call.
         *
         * @note If execution has been successful, then pushed params are reset.
         *
         * @note Param result can be nullptr only if exec type of forward is ignore.
         *
         * @param result    Address where the result will be stored.
         *
         * @return          True if succeed, false if execution failed.
         */
        virtual bool execFunc(ReturnValue *result) = 0;

        /*
         * @brief Resets params already pushed to forward.
         *
         * @noreturn
         */
        virtual void resetParams() = 0;

    protected:
        virtual ~IForward() = default;
    };

    /**
     * @brief Callback gets executed when forward is being executed.
     * 
     * @param fwd      Forward that is being executed.
     * @param params   Params passed to forward.
     * 
     * @return         If forward is meant for a single plugin then it should return ExecType::Ignore, otherwise return value should be the same value returned by plugins.
     */
    using ForwardCallback = IForward::ReturnValue (*)(const IForward *const fwd, IForward::Param *params);

    class IForwardMngr SPMOD_FINAL : public ISPModInterface
    {
    public:
        static constexpr uint16_t MAJOR_VERSION = 0;
        static constexpr uint16_t MINOR_VERSION = 0;

        static constexpr uint32_t VERSION = (MAJOR_VERSION << 16 | MINOR_VERSION);
        /**
         * @brief Gets interface's name.
         *
         * @return        Interface's name.
         */
        const char *getName() const override
        {
            return "IForwardMngr";
        }

        /**
         * @brief Gets interface's version.
         *
         * @note The first 16 most significant bits represent major version, the rest represent minor version.
         *
         * @return        Interface's version.
         */
        uint32_t getVersion() const override
        {
            return VERSION;
        }

        /*
         * @brief Creates forward.
         *
         * @note  Name of the forward is also the name of function
         *        that will be executed in a plugin.
         *
         * @param name      Name of the forward.
         * @param exec      Exec type.
         * @param params    Number of parameters, cannot exceed MAX_EXEC_PARAMS.
         * @param ...       Types of parameters (IForward::ParamType).
         *
         * @return          Forward pointer, nullptr if failed.
         */
        virtual IForward *createForward(const char *name,
                                        IForward::ExecType exec,
                                        std::size_t params,
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
                                        std::size_t params,
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
         * @brief Adds listener.
         *
         * @param func   Function to be called.
         *
         * @noreturn
         */
        virtual void addForwardListener(ForwardCallback func) = 0;

    protected:
        virtual ~IForwardMngr() = default;
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
}
