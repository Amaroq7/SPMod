/*
 *  Copyright (C) 2019-2020 SPMod Development Team
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

namespace SPMod
{
    enum class MsgDest : std::uint8_t;
}

namespace SPMod::Engine
{
    class IFuncs
    {
    public:
        virtual ~IFuncs() = default;

        /**
         * @brief Returns current command's argument.
         *
         * @param arg          Argument to get.
         *
         * @return Command's argument.
         */
        virtual std::string_view getArg(std::uint32_t arg) const = 0;

        /**
         * @brief Returns current command's arguments.
         *
         * @return Command's arguments.
         */
        virtual std::string_view getArgs() const = 0;

        /**
         * @brief Returns current command's arguments count.
         *
         * @param arg          Argument to get.
         *
         * @return Command's arguments count.
         */
        virtual std::uint32_t getArgc() const = 0;

        /**
         * @brief Prints message to server's console.
         *
         * @param msg          Message to print to the server's console.
         *
         * @noreturn
         */
        virtual void serverPrint(std::string_view msg) const = 0;

        /**
         * @brief Precaches model.
         *
         * @param model          Model to precache.
         *
         * @return Precached model's index.
         */
        virtual std::uint32_t precacheModel(std::string_view model) const = 0;

        /**
         * @brief Precaches sound.
         *
         * @param sound          Sound to precache.
         *
         * @return Precached sound's index.
         */
        virtual std::uint32_t precacheSound(std::string_view sound) const = 0;

        /**
         * @brief Precaches generic resource.
         *
         * @param generic          Generic resource to precache.
         *
         * @return Precached generic resource's index.
         */
        virtual std::uint32_t precacheGeneric(std::string_view generic) const = 0;

        /**
         * @brief Changes level.
         *
         * @param generic          Level to change to.
         *
         */
        virtual void changeLevel(std::string_view level) const = 0;

        /**
         * @brief Executes server's command.
         *
         * @note Executes command in the next frame. To get the result of the command use serverExecute() afterwards.
         *
         * @param cmd          Command to execute.
         *
         */
        virtual void serverCommand(std::string_view cmd) const = 0;

        /**
         * @brief Executes previously buffered server's command.
         *
         */
        virtual void serverExecute() const = 0;

        /**
         * @brief Registers server's command.
         *
         * @param cmd          Command to register.
         * @param callback     Function to be executed when command is used.
         *
         */
        virtual void registerSrvCommand(std::string_view cmd, ServerCmdCallback callback) const = 0;

        // TODO: Describe funcs
        virtual void messageBegin(MsgDest msgDest,
                                  std::uint32_t msgType,
                                  const float *pOrigin = nullptr,
                                  IEdict *pEdict = nullptr) const = 0;
        virtual void messageEnd() const = 0;
        virtual void writeByte(std::int32_t byteArg) const = 0;
        virtual void writeChar(std::int32_t charArg) const = 0;
        virtual void writeShort(std::int32_t shortArg) const = 0;
        virtual void writeLong(std::int32_t longArg) const = 0;
        virtual void writeEntity(std::int32_t entArg) const = 0;
        virtual void writeAngle(float angleArg) const = 0;
        virtual void writeCoord(float coordArg) const = 0;
        virtual void writeString(std::string_view strArg) const = 0;
    };
} // namespace SPMod::Engine
