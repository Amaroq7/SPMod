/*
 *  Copyright (C) 2018-2020 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  SPMod is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with SPMod.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

namespace SPMod
{
    class IPlugin;
    class IForward
    {
    public:
        /*
         * Execution types
         */
        enum class ExecType : std::uint8_t
        {
            /* Ignore returned result */
            Ignore = 0,

            /* Stop executing when plugin return ReturnValue::PluginStop */
            Stop = (1 << 0),

            /* Return the highest value returned by plugins */
            Highest = (1 << 1)
        };

        /*
         * String flags for string param
         */
        enum class StringFlags : std::uint8_t
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

        class IParam
        {
        public:
            /*
             * Param types
             */
            enum class Type : std::uint8_t
            {
                None = 0,
                Array = (1 << 0),
                Pointer = (1 << 1),
                Int = (1 << 2),
                Float = (1 << 3),
                String = (1 << 4)
            };

            virtual std::any getData() const = 0;
            virtual bool shouldCopyback() const = 0;
            virtual StringFlags getStringFlags() const = 0;
            virtual Type getDataType() const = 0;
            virtual std::size_t getDataSize() const = 0;

            virtual ~IParam() = default;
        };

        /**
         * @brief Callback gets executed when forward is being executed.
         *
         * @param fwd      Forward that is being executed.
         * @param result   Result of executing where it should be written to.
         * @param stop     Should be set to true if further callbacks should not be called. (multi forward only)
         *
         * @noreturn
         */
        using Callback = std::function<void(IForward *const fwd, std::int32_t &result, bool &stop)>;

        /*
         * Predefined values that can be returned by plugins
         */
        enum class ReturnValue : std::uint8_t
        {
            Ignored = 0,
            Stop,
            Handled,
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
        virtual std::string_view getName() const = 0;

        /*
         * @brief Returns plugin which the forward will be executed in.
         *
         * @return      Plugin pointer, nullptr if forward will be executed in all plugins.
         */
        virtual IPlugin *getPlugin() const = 0;

        /*
         * @brief Gets execution type of the forward.
         *
         * @return      Execution type.
         */
        virtual ExecType getExecType() const = 0;

        /*
         * @brief Returns param.
         *
         * @param id    Param id.
         *
         * @return      Param.
         */
        std::array<IParam *, MAX_EXEC_PARAMS> getParams() const
        {
            return getParamsImpl();
        }

        /*
         * @brief Pushes int to the current call.
         *
         * @param integer Integer to pass.
         *
         * @return        True if succeed, false if parameter type is wrong.
         */
        virtual bool pushInt(std::int32_t integer) = 0;

        /*
         * @brief Pushes integer pointer to the current call.
         *
         * @param integer   Integer pointer to pass.
         * @param copyback  True if copy back value, false to not.
         *
         * @return          True if succeed, false if parameter type is wrong.
         */
        virtual bool pushInt(std::int32_t *integer, bool copyback) = 0;

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
        virtual bool pushFloat(float *real, bool copyback) = 0;

        /*
         * @brief Pushes array to the current call.
         *
         * @param array     Array to pass.
         * @param size      Size of the array.
         * @param copyback  True if copy back value, false to not.
         *
         * @return          True if succeed, false if parameter type is wrong.
         */
        virtual bool pushArray(std::variant<std::int32_t *, float *> array, std::size_t size, bool copyback) = 0;

        /*
         * @brief Pushes string to the current call.
         *
         * @param string    String to pass.
         *
         * @return          True if succeed, false if parameter type is wrong.
         */
        virtual bool pushString(std::string_view string) = 0;

        /*
         * @brief Pushes string to the current call.
         *
         * @param buffer    String to pass.
         * @param size      Size of the string.
         * @param sflags    String flags.
         * @param copyback  True if copy back value, false to not.
         *
         * @return          True if succeed, false if parameter type is wrong.
         */
        virtual bool pushString(char *buffer, std::size_t size, StringFlags sflags, bool copyback) = 0;

        /*
         * @brief Pushes string to the current call.
         *
         * @note If execution has been successful, then pushed params are reset.
         * @note Param result can be nullptr only if exec type of forward is ignore.
         *
         * @param result    Address where the result will be stored.
         *
         * @return          True if succeed, false if execution failed.
         */
        virtual bool execFunc(std::int32_t *result) = 0;

        /*
         * @brief Resets params already pushed to forward.
         *
         * @noreturn
         */
        virtual void resetParams() = 0;

        virtual ~IForward() = default;

    protected:
        virtual std::array<IParam *, MAX_EXEC_PARAMS> getParamsImpl() const = 0;
    };

    class IForwardMngr : public ISPModInterface
    {
    public:
        static constexpr std::uint16_t MAJOR_VERSION = 0;
        static constexpr std::uint16_t MINOR_VERSION = 0;

        static constexpr std::uint32_t VERSION = (MAJOR_VERSION << 16 | MINOR_VERSION);
        /**
         * @brief Gets interface's name.
         *
         * @return        Interface's name.
         */
        std::string_view getName() const override
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
        std::uint32_t getVersion() const override
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
         * @param params    Types of parameters (IForward::Param::Type).
         *
         * @return          Forward pointer, nullptr if failed.
         */
        IForward *createForward(std::string_view name,
                                IForward::ExecType exec = IForward::ExecType::Ignore,
                                std::array<IForward::IParam::Type, IForward::MAX_EXEC_PARAMS> params = {})
        {
            return createForward(name, exec, params, nullptr);
        }

        /*
         * @brief Creates forward.
         *
         * @note  Name of the forward is also the name of function
         *        that will be executed in a plugin.
         *
         * @param name      Name of the forward.
         * @param plugin    Plugin which the forward will be executed in.
         * @param params    Types of parameters (IForward::Param::Type).
         *
         * @return          Forward pointer, nullptr if failed.
         */
        IForward *createForward(std::string_view name,
                                IPlugin *plugin,
                                std::array<IForward::IParam::Type, IForward::MAX_EXEC_PARAMS> params)
        {
            return createForward(name, IForward::ExecType::Ignore, params, plugin);
        }

        /*
         * @brief Creates forward.
         *
         * @note  Name of the forward is also the name of function
         *        that will be executed in a plugin.
         *
         * @param name      Name of the forward.
         * @param exec      Exec type.
         * @param plugin    Plugin which the forward will be executed in.
         * @param params    Types of parameters (IForward::Param::Type).
         *
         * @return          Forward pointer, nullptr if failed.
         */
        virtual IForward *createForward(std::string_view name,
                                        IForward::ExecType exec,
                                        std::array<IForward::IParam::Type, IForward::MAX_EXEC_PARAMS> params,
                                        IPlugin *plugin) = 0;

        /*
         * @brief Deletes forward.
         *
         * @note Forward will not be deleted if it is being executed.
         *
         * @param forward   Forward to remove.
         *
         * @return          True if forward has been successfully deleted, false otherwise.
         */
        virtual bool deleteForward(const IForward *forward) = 0;

        /*
         * @brief Adds listener.
         *
         * @param func   Function to be called.
         *
         * @noreturn
         */
        virtual void addForwardListener(IForward::Callback func) = 0;

        virtual ~IForwardMngr() = default;
    };

    template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    inline bool hasEnumFlag(const T type, const T flag)
    {
        return static_cast<bool>(static_cast<std::underlying_type_t<T>>(type) &
                                 static_cast<std::underlying_type_t<T>>(flag));
    }

    template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    inline T operator&(const T lhs, const T rhs)
    {
        return static_cast<T>(static_cast<std::underlying_type_t<T>>(lhs) &
                              static_cast<std::underlying_type_t<T>>(rhs));
    }

    template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    inline T operator&=(const T lhs, const T rhs)
    {
        auto returnVal = static_cast<std::underlying_type_t<T>>(lhs);
        returnVal &= static_cast<std::underlying_type_t<T>>(rhs);

        return static_cast<T>(returnVal);
    }

    template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    inline T operator|(const T lhs, const T rhs)
    {
        return static_cast<T>(static_cast<std::underlying_type_t<T>>(lhs) |
                              static_cast<std::underlying_type_t<T>>(rhs));
    }

    template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    inline T operator|=(const T lhs, const T rhs)
    {
        auto returnVal = static_cast<std::underlying_type_t<T>>(lhs);
        returnVal |= static_cast<std::underlying_type_t<T>>(rhs);

        return static_cast<T>(returnVal);
    }

    template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    inline T operator^(const T lhs, const T rhs)
    {
        return static_cast<T>(static_cast<std::underlying_type_t<T>>(lhs) ^
                              static_cast<std::underlying_type_t<T>>(rhs));
    }

    template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
    inline T operator^=(const T lhs, const T rhs)
    {
        auto returnVal = static_cast<std::underlying_type_t<T>>(lhs);
        returnVal ^= static_cast<std::underlying_type_t<T>>(rhs);

        return static_cast<T>(returnVal);
    }

    template<typename T, typename S, typename = std::enable_if_t<std::is_enum_v<T>>>
    inline bool operator==(const T lhs, const S rhs)
    {
        return static_cast<std::underlying_type_t<T>>(lhs) == rhs;
    }

    template<typename T, typename S, typename = std::enable_if_t<std::is_enum_v<T>>>
    inline bool operator==(const S lhs, const T rhs)
    {
        return lhs == static_cast<std::underlying_type_t<T>>(rhs);
    }

    template<typename T, typename S, typename = std::enable_if_t<std::is_enum_v<T>>>
    inline bool operator!=(const T lhs, const S rhs)
    {
        return static_cast<std::underlying_type_t<T>>(lhs) != rhs;
    }

    template<typename T, typename S, typename = std::enable_if_t<std::is_enum_v<T>>>
    inline bool operator!=(const S lhs, const T rhs)
    {
        return lhs != static_cast<std::underlying_type_t<T>>(rhs);
    }
} // namespace SPMod
