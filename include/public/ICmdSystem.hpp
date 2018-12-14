/*
*  Copyright (C) 2018 SPMod Development Team
*
*  This file is part of SPMod.
*
*  SPMod is free software: you can redistribute it and/or modify
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
    class IPlayer;

    class ICommand SPMOD_FINAL
    {
    public:
        enum class Type : std::uint8_t
        {
            Client = 0,
            Server = 1
        };

        /**
         * @brief Returns command's name.
         * 
         * @return Command's name.
         */
        virtual const char *getCmd() const = 0;

        /**
         * @brief Returns command's info.
         * 
         * @return Command's info.
         */
        virtual const char *getInfo() const = 0;

        /**
         * @brief Checks if player can execute the command.
         * 
         * @return True if player has access, false otherwise.
         */
        virtual bool hasAccess(IPlayer *player) const = 0;

        /**
         * @brief Returns command's access flags.
         * 
         * @return Command's flags (bitwise).
         */
        virtual std::uint32_t getAccess() const = 0;

    protected:
        virtual ~ICommand() = default;
    };

    class ICommandMngr SPMOD_FINAL : public ISPModInterface
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
        const char *getName() const override
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
        std::uint32_t getVersion() const override
        {
            return VERSION;
        }

        /**
         * @brief Registers a command.
         * 
         * @param type          Command type.
         * @param cmd           Command name.
         * @param info          Command info.
         * @param flags         Access flags.
         * 
         * @return        Registered command.
         */
        virtual ICommand *registerCommand(ICommand::Type type, const char *cmd, const char *info, std::uint32_t flags) = 0;

    protected:
        virtual ~ICommandMngr() = default;
    };
}
