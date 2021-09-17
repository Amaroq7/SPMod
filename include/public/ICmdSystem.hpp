/*
 *  Copyright (C) 2018-2020 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SPMod is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with SPMod.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "StandardHeaders.hpp"
#include "IInterface.hpp"
#include "IForwardSystem.hpp"

namespace SPMod
{
    class IPlayer;

    class ICommand
    {
    public:
        using Callback = std::function<bool(nstd::observer_ptr<IPlayer> player, nstd::observer_ptr<ICommand> cmd)>;

        enum class Type : std::uint8_t
        {
            Client,
            Server
        };

        virtual ~ICommand() = default;

        /**
         * @brief Returns command's name or regex, depends on how command was registered.
         *
         * @note Always returns name for server commands.
         *
         * @return Command's regex or name.
         */
        [[nodiscard]] virtual const std::variant<std::string, std::regex> &getNameOrRegex() const = 0;

        /**
         * @brief Returns command's info.
         *
         * @return Command's info.
         */
        [[nodiscard]] virtual std::string_view getInfo() const = 0;

        /**
         * @brief Checks if player can execute the command.
         *
         * @param player          Player to check access.
         *
         * @return True if player has access, false otherwise.
         */
        [[nodiscard]] virtual bool hasAccess(nstd::observer_ptr<IPlayer> player) const = 0;

        /**
         * @brief Returns permission to execute command.
         *
         * @return Permission name.
         */
        [[nodiscard]] virtual std::string_view getPermission() const = 0;
    };

    class ICommandMngr : public ISPModInterface
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
            return "ICommandMngr";
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

        ~ICommandMngr() override = default;

        /**
         * @brief Registers a command.
         *
         * @note Param regex can be only true for client commands.
         *
         * @param type          Command type.
         * @param cmd           Command name.
         * @param info          Command info.
         * @param regex         Register command name as regex.
         * @param permission    Permission to execute command.
         * @param cb            Callback.
         *
         * @return        Registered command.
         */
        virtual nstd::observer_ptr<ICommand> registerCommand(ICommand::Type type,
                                                        std::string_view cmd,
                                                        std::string_view info,
                                                        bool regex,
                                                        std::string_view permission,
                                                        ICommand::Callback cb) = 0;
    };
} // namespace SPMod
