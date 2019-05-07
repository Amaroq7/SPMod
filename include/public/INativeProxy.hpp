/*
 *  Copyright (C) 2018 SPMod Development Team
 *
 *  This file is part of SPMod.
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

namespace SPMod
{
    class IPlugin;

    class IProxiedNative SPMOD_FINAL
    {
    public:
        /**
         * @brief Gets native name.
         *
         * @return      Native's name.
         */
        virtual const char *getName() const = 0;

        /**
         * @brief Gets native data.
         *
         * @return      Native's data.
         */
        virtual void *getData() const = 0;

        /**
         * @brief Gets native plugin.
         *
         * @return      Native's plugin.
         */
        virtual const IPlugin *getPlugin() const = 0;
    
    protected:
        ~IProxiedNative() = default;
    };

    class INativeProxy SPMOD_FINAL : public ISPModInterface
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
        const char *getName() const override
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
        virtual bool registerNative(const char *name,
                                    void *data,
                                    const IPlugin *plugin) = 0;

        /**
         * @brief Notifies extension about executed native.
         * 
         * @param native    Executed native.
         * @param plugin    Plugin which has executed the native.
         *
         * @return          Native result.
         */
        virtual int nativeExecNotify(const IProxiedNative *native,
                                     const IPlugin *const plugin) const = 0;

        /**
         * @brief Gets proxied native.
         * 
         * @param native    Native's id.
         *
         * @return          Proxied native or nullptr if invalid id.
         */
        virtual const IProxiedNative *getProxiedNative(std::size_t id) const = 0;

        /**
         * @brief Returns number of proxied natives.
         *
         * @return          Number of natives.
         */
        virtual std::size_t getNativesNum() const = 0;
    };
}