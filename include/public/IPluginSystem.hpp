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
    class IPluginMngr;

    class IPlugin
    {
    public:
        /**
         * @brief Returns name of plugin.
         *
         * @return        Plugin name.
         */
        virtual const char *getName() const = 0;

        /**
         * @brief Returns version of plugin.
         *
         * @return        Plugin version.
         */
        virtual const char *getVersion() const = 0;

        /**
         * @brief Returns author of plugin.
         *
         * @return        Plugin author.
         */
        virtual const char *getAuthor() const = 0;

        /**
         * @brief Returns url address of plugin.
         *
         * @return        Plugin url address.
         */
        virtual const char *getUrl() const = 0;

        /**
         * @brief Returns identity of plugin.
         *
         * @note          It's the same as getFilename() except it doesn't contain file extension.
         *
         * @return        Plugin identity.
         */
        virtual const char *getIdentity() const = 0;

        /**
         * @brief Returns filename of plugin.
         *
         * @return        Plugin filename.
         */
        virtual const char *getFilename() const = 0;

        /**
         * @brief Gets plugin's plugin manager.
         * 
         * @return        Plugin manager.
         */
        virtual const IPluginMngr *getPluginMngr() const = 0;

    protected:
        virtual ~IPlugin() = default;
    };

    class IPluginMngr : public ISPModInterface
    {
    public:
        static constexpr std::uint16_t MAJOR_VERSION = 0;
        static constexpr std::uint16_t MINOR_VERSION = 0;

        static constexpr std::uint32_t VERSION = (MAJOR_VERSION << 16 | MINOR_VERSION);

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

        /**
         * @brief Searches for plugin by specified identity.
         *
         * @param name    Identity to search for.
         *
         * @return        Plugin pointer, nullptr if not found.
         */
        virtual IPlugin *getPlugin(const char *name) = 0;

        /**
         * @brief Returns numbers of loaded plugins.
         *
         * @return        Number of loaded plugins.
         */
        virtual std::size_t getPluginsNum() const = 0;

    protected:
        virtual ~IPluginMngr() = default;
    };
}
