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
#include "Common.hpp"

#include <IForwardSystem.hpp>
#include <ITimerSystem.hpp>
#include <IMenuSystem.hpp>
#include <IUtilsSystem.hpp>
#include <IPlayerSystem.hpp>
#include <ILoggerSystem.hpp>
#include <ICmdSystem.hpp>
#include <IPluginSystem.hpp>
#include <INativeProxy.hpp>
#include <IMessageSystem.hpp>
#include <IAccessSystem.hpp>
#include <IHooks.hpp>

#include <anubis/observer_ptr.hpp>

namespace SPMod
{
    class ISPGlobal : public ISPModInterface
    {
    public:
        static constexpr std::uint16_t MAJOR_VERSION = 0;
        static constexpr std::uint16_t MINOR_VERSION = 0;

        static constexpr std::uint32_t VERSION = (MAJOR_VERSION << 16 | MINOR_VERSION);

        /**
         * @brief Gets interface's name.
         *
         * @return      Interface's name.
         */
        [[nodiscard]] std::string_view getName() const override
        {
            return "ISPGlobal";
        }

        /**
         * @brief Gets interface's version.
         *
         * @note The first 16 most significant bits represent major version, the rest represent minor version.
         *
         * @return      Interface's version.
         */
        [[nodiscard]] std::uint32_t getVersion() const override
        {
            return VERSION;
        }

        ~ISPGlobal() override = default;

        /**
         * @brief Returns home dir of SPMod.
         *
         * @return              Home dir.
         */
        [[nodiscard]] virtual const std::filesystem::path &getPath(DirType type) const = 0;

        /**
         * @brief Finds plugin.
         *
         * @param pluginname    Plugin name to look up.
         *
         * @return              Plugin pointer or nullptr if not found.
         */
        [[nodiscard]] virtual nstd::observer_ptr<IPlugin> getPlugin(std::string_view pluginname) const = 0;

        /**
         * @brief Returns SPMod forward manager.
         *
         * @return              Forward manager.
         */
        [[nodiscard]] virtual nstd::observer_ptr<IForwardMngr> getForwardManager() const = 0;

        /**
         * @brief Returns SPMod timer manager.
         *
         * @return              Timer manager.
         */
        [[nodiscard]] virtual nstd::observer_ptr<ITimerMngr> getTimerManager() const = 0;

        /**
         * @brief Returns SPMod menu manager.
         *
         * @return              Menu manager.
         */
        [[nodiscard]] virtual nstd::observer_ptr<IMenuMngr> getMenuManager() const = 0;

        /**
         * @brief Returns SPMod message manager.
         *
         * @return              Message manager.
         */
        [[nodiscard]] virtual nstd::observer_ptr<IMessageMngr> getMessageManager() const = 0;

        /**
         * @brief Returns SPMod logger manager.
         *
         * @return              Logger manager.
         */
        [[nodiscard]] virtual nstd::observer_ptr<ILoggerMngr> getLoggerManager() const = 0;

        /**
         * @brief Return SPMod player manager.
         *
         * @return              Player manager.
         */
        [[nodiscard]] virtual nstd::observer_ptr<IPlayerMngr> getPlayerManager() const = 0;

        /**
         * @brief Return SPMod utils funcs.
         *
         * @return              Utils funcs.
         */
        [[nodiscard]] virtual nstd::observer_ptr<IUtils> getUtils() const = 0;

        /**
         * @brief Return native proxy.
         *
         * @return              Native proxy.
         */
        [[nodiscard]] virtual nstd::observer_ptr<INativeProxy> getNativeProxy() const = 0;

        /**
         * @brief Returns SPMod command manager.
         *+
         * @return              Command manager.
         */
        [[nodiscard]] virtual nstd::observer_ptr<ICommandMngr> getCommandManager() const = 0;

        /**
         * @brief Returns SPMod group access manager.
         *
         * @return              Group access manager.
         */
        [[nodiscard]] virtual nstd::observer_ptr<IGroupMngr> getGroupAccessManager() const = 0;

        [[nodiscard]] virtual nstd::observer_ptr<IHooks> getHooks() const = 0;

        /**
         * @brief Registers module's interface.
         *
         * @param interface     Interface's implementation.
         *
         * @return              True if registered successfully, false otherwise.
         */
        virtual bool registerModule(nstd::observer_ptr<IModuleInterface> interface) = 0;

        /**
         * @brief Registers adapter's interface.
         *
         * @param interface     Adapter's implementation.
         *
         * @return              True if registered successfully, false otherwise.
         */
        virtual bool registerAdapter(nstd::observer_ptr<IAdapterInterface> interface) = 0;

        /**
         * @brief Gets a module's interface.
         *
         * @param name          Name of the interface to look up for.
         *
         * @return              Interface's implementation.
         */
        [[nodiscard]] virtual nstd::observer_ptr<IModuleInterface> getInterface(std::string_view name) const = 0;
    };
} // namespace SPMod
