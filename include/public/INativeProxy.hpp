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

    class IProxiedNative
    {
    public:
        /**
         * @brief Gets native name.
         *
         * @return      Native's name.
         */
        virtual std::string_view getName() const = 0;

        /**
         * @brief Gets native data.
         *
         * @return      Native's data.
         */
        virtual std::any getData() const = 0;

        /**
         * @brief Gets native plugin.
         *
         * @return      Native's plugin.
         */
        virtual IPlugin *getPlugin() const = 0;

        /**
         * @brief Notifies adapter about executed native.
         *
         * @param plugin    Plugin which has executed the native.
         *
         * @return          Native result.
         */
        virtual std::int32_t exec(IPlugin *plugin) = 0;

        virtual ~IProxiedNative() = default;
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

        /**
         * @brief Registers proxied native.
         *
         * @param name      Native name.
         * @param data      Native data.
         * @param plugin    Plugin which native belongs to.
         *
         * @return          True if registration succeed, false otherwise.
         */
        virtual bool registerNative(std::string_view name, std::any data, IPlugin *plugin) = 0;

        /**
         * @brief Gets list of proxied natives.
         *
         * @param native    Native's id.
         *
         * @return          Proxied native or nullptr if invalid id.
         */
        std::vector<IProxiedNative *> getProxiedNatives() const
        {
            return getProxiedNativesImpl();
        }

        virtual ~INativeProxy() = default;

    protected:
        virtual std::vector<IProxiedNative *> getProxiedNativesImpl() const = 0;
    };
} // namespace SPMod