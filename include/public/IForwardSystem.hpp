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

#include "StandardHeaders.hpp"
#include "IInterface.hpp"

namespace SPMod
{
    class IPlugin;
    class IForward
    {
    public:
        enum class Status : std::uint8_t
        {
            Continue = 0,
            Changed,
            Handled,
            Stop
        };

        enum class Type : std::uint8_t
        {
            Private = 0,
            Global
        };

        /*
         * Execution types
         */
        enum class ExecType : std::uint8_t
        {
            /* Ignore returned result */
            Ignore = 0,

            /* Only return the last exec, ignore all others */
            Single = (1 << 0),

            /* Returns the highest value, no mid-Stops allowed */
            Event = (1 << 1),

            /* Returns the highest value, mid-Stops allowed */
            Hook = (1 << 2)
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
            using Variants = std::variant<std::int32_t,
                                          float,
                                          std::int32_t *,
                                          float *,
                                          std::string_view,
                                          std::reference_wrapper<std::string>,
                                          std::reference_wrapper<std::vector<std::variant<std::int32_t, float>>>>;

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

        public:
            virtual ~IParam() = default;
            [[nodiscard]] virtual Variants getData() const = 0;
            [[nodiscard]] virtual bool shouldCopyback() const = 0;
            [[nodiscard]] virtual StringFlags getStringFlags() const = 0;
        };

        /**
         * @brief Callback gets executed when forward is being executed.
         *
         * @param fwd      Forward that is being executed.
         *
         * @noreturn
         */
        using Callback = std::function<Status(nstd::observer_ptr<IForward> fwd)>;

        static constexpr std::size_t MAX_EXEC_PARAMS = 32;

        virtual ~IForward() = default;

        /*
         * @brief Returns forward name.
         *
         * @note  Name of the forward is also the name of function
         *        that will be executed in a plugin.
         *
         * @return      Null-terminated name.
         */
        [[nodiscard]] virtual std::string_view getName() const = 0;

        /*
         * @brief Returns forward type.
         *
         * @return      Forward type.
         */
        [[nodiscard]] virtual Type getType() const = 0;

        /*
         * @brief Gets execution type of the forward.
         *
         * @return      Execution type.
         */
        [[nodiscard]] virtual ExecType getExecType() const = 0;

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
         *
         * @return          True if succeed, false if parameter type is wrong.
         */
        virtual bool pushInt(std::int32_t *integer) = 0;

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
         *
         * @return          True if succeed, false if parameter type is wrong.
         */
        virtual bool pushFloat(float *real) = 0;

        /*
         * @brief Pushes array to the current call.
         *
         * @param array     Array to pass.
         *
         * @return          True if succeed, false if parameter type is wrong.
         */
        virtual bool pushArray(std::vector<std::variant<std::int32_t, float>> &array) = 0;

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
         * @param string    String to pass.
         * @param sflags    String flags.
         * @param copyback  True if copy back value, false to not.
         *
         * @return          True if succeed, false if parameter type is wrong.
         */
        virtual bool pushString(std::string &string, StringFlags sflags, bool copyback) = 0;

        /*
         * @brief Resets params already pushed to forward.
         *
         * @noreturn
         */
        virtual void resetParams() = 0;

        virtual void addFunction(IForward::Callback func) = 0;
        virtual void removeFunction(IForward::Callback func) = 0;
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
        [[nodiscard]] std::string_view getName() const override
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
        [[nodiscard]] std::uint32_t getVersion() const override
        {
            return VERSION;
        }

        ~IForwardMngr() override = default;

        /*
         * @brief Creates global forward.
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
        virtual nstd::observer_ptr<IForward> createForward(std::string_view name,
                                        IForward::ExecType exec,
                                        std::array<IForward::IParam::Type, IForward::MAX_EXEC_PARAMS> params) = 0;

        /*
         * @brief Creates private forward.
         *
         * @note  Name of the forward is also the name of function
         *        that will be executed in a plugin.
         *
         * @param exec      Exec type.
         * @param params    Types of parameters (IForward::Param::Type).
         *
         * @return          Forward pointer, nullptr if failed.
         */
        virtual nstd::observer_ptr<IForward> createForward(
            IForward::ExecType exec,
            std::array<IForward::IParam::Type, IForward::MAX_EXEC_PARAMS> params
        ) = 0;


        virtual bool execForward(nstd::observer_ptr<IForward> forward, IForward::Status *result) = 0;

        /*
         * @brief Deletes forward.
         *
         * @note Forward will not be deleted if it is being executed.
         *
         * @param forward   Forward to remove.
         *
         * @return          True if forward has been successfully deleted, false otherwise.
         */
        virtual bool deleteForward(nstd::observer_ptr<IForward> forward) = 0;
    };
} // namespace SPMod
