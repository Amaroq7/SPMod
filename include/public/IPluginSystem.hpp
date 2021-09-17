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

#include <anubis/observer_ptr.hpp>

namespace SPMod
{
    class IPluginMngr;
    class IProxiedNative;

    class IPlugin
    {
    public:
        virtual ~IPlugin() = default;

        /**
         * @brief Returns name of plugin.
         *
         * @return        Plugin name.
         */
        [[nodiscard]] virtual std::string_view getName() const = 0;

        /**
         * @brief Returns version of plugin.
         *
         * @return        Plugin version.
         */
        [[nodiscard]] virtual std::string_view getVersion() const = 0;

        /**
         * @brief Returns author of plugin.
         *
         * @return        Plugin author.
         */
        [[nodiscard]] virtual std::string_view getAuthor() const = 0;

        /**
         * @brief Returns url address of plugin.
         *
         * @return        Plugin url address.
         */
        [[nodiscard]] virtual std::string_view getUrl() const = 0;

        /**
         * @brief Returns identity of plugin.
         *
         * @note          It's the same as getFilename() except it doesn't contain file extension.
         *
         * @return        Plugin identity.
         */
        [[nodiscard]] virtual std::string_view getIdentity() const = 0;

        /**
         * @brief Returns filename of plugin.
         *
         * @return        Plugin filename.
         */
        [[nodiscard]] virtual std::string_view getFilename() const = 0;

        /**
         * @brief Gets plugin's plugin manager.
         *
         * @return        Plugin manager.
         */
        [[nodiscard]] virtual nstd::observer_ptr<IPluginMngr> getPluginMngr() const = 0;

        [[nodiscard]] virtual bool functionExist(std::string_view funcName) const = 0;
        [[nodiscard]] virtual IForward::Status functionCall(nstd::observer_ptr<IForward> fwd) const = 0;

        [[nodiscard]] std::size_t getId() const
        {
            return m_pluginId;
        }

#if defined SPMOD_CORE
        void setId(std::size_t id)
        {
            m_pluginId = id;
        }
#endif
    protected:
        std::size_t m_pluginId;
    };

    class IPluginMngr
    {
    public:
        virtual ~IPluginMngr() = default;

        /**
         * @brief Searches for plugin by specified identity.
         *
         * @param name    Identity to search for.
         *
         * @return        Plugin pointer, nullptr if not found.
         */
        [[nodiscard]] virtual nstd::observer_ptr<IPlugin> getPlugin(std::string_view name) const = 0;

        /**
         * @brief Loads plugins.
         *
         */
        virtual void loadPlugins() = 0;

        /**
         * @brief Binds natives created by other plugins.
         *
         */
        virtual void bindPluginsNatives() = 0;

        /**
         * @brief Returns numbers of loaded plugins.
         *
         * @return        Number of loaded plugins.
         */
        [[nodiscard]] virtual std::size_t getPluginsNum() const = 0;

        /**
         * @brief Returns plugins extension.
         *
         * @return        Plugins extension.
         */
        [[nodiscard]] virtual std::string_view getPluginsExt() const = 0;

        /**
         * @brief Returns list of the plugins.
         *
         * @return        Plugins list.
         */
        [[nodiscard]] virtual const std::vector<nstd::observer_ptr<IPlugin>> &getPluginsList() const = 0;
    };
} // namespace SPMod
