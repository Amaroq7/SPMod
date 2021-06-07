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

    class IProxiedNative
    {
    public:
        virtual ~IProxiedNative() = default;

        /**
         * @brief Gets native name.
         *
         * @return      Native's name.
         */
        virtual std::string_view getName() const = 0;

        /**
         * @brief Gets passed param as int.
         *
         * @param index   Param number.
         *
         * @return        Param as int.
         */
        virtual std::int32_t getParamAsInt(std::size_t index) const = 0;

        /**
         * @brief Gets passed param as pointer to int.
         *
         * @param index   Param number.
         *
         * @return        Param as pointer to int.
         */
        virtual std::int32_t *getParamAsIntAddr(std::size_t index) const = 0;

        /**
         * @brief Gets passed param as float.
         *
         * @param index   Param number.
         *
         * @return        Param as float.
         */
        virtual float getParamAsFloat(std::size_t index) const = 0;

        /**
         * @brief Gets passed param as pointer to float.
         *
         * @param index   Param number.
         *
         * @return        Param as pointer to float.
         */
        virtual float *getParamAsFloatAddr(std::size_t index) const = 0;

        /**
         * @brief Gets passed param as string.
         *
         * @param index   Param number.
         *
         * @return        Param as string.
         */
        virtual char *getParamAsString(std::size_t index) const = 0;

        /**
         * @brief Gets passed param as array.
         *
         * @param index   Param number.
         *
         * @return        Param as pointer to float.
         */
        virtual void *getParamAsArray(std::size_t index) const = 0;

        /**
         * @brief Executes the native.
         *
         * @return        Param as pointer to float.
         */
        virtual std::int32_t InvokeSPModNative(IPlugin *plugin) = 0;
    };

    class INativeProxy : public ISPModInterface
    {
    public:
        static constexpr std::uint16_t MAJOR_VERSION = 0;
        static constexpr std::uint16_t MINOR_VERSION = 0;

        static constexpr std::uint32_t VERSION = (MAJOR_VERSION << 16 | MINOR_VERSION);

        /**
         * @brief Gets interface's name.
         *
         * @return          Interface's name.
         */
        std::string_view getName() const override
        {
            return "INativeProxy";
        }

        /**
         * @brief Gets interface's version.
         *
         * @note The first 16 most significant bits represent major version, the rest represent minor version.
         *
         * @return          Interface's version.
         */
        std::uint32_t getVersion() const override
        {
            return VERSION;
        }

        virtual ~INativeProxy() = default;

        /**
         * @brief Registers proxied native.
         *
         * @param name      Native name.
         * @param callback  Callback to be executed.
         *
         * @return          True if registration succeed, false otherwise.
         */
        virtual bool registerNative(IProxiedNative *native) = 0;

        /**
         * @brief Returns list of already registered proxied natives.
         *
         * @return          Map of registered proxied natives (first element - name of the native, second - implementation)
         */
        virtual const std::unordered_map<std::string, IProxiedNative *> &getProxiedNatives() const = 0;
    };
} // namespace SPMod