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
    enum class MessageDest : std::uint8_t
    {
        BROADCAST = 0,      // unreliable to all
        ONE = 1,            // reliable to one (msg_entity)
        ALL = 2,            // reliable to all
        INIT = 3,           // write to the init string
        PVS = 4,            // Ents in PVS of org
        PAS = 5,            // Ents in PAS of org
        PVS_R = 6,          // Reliable to PVS
        PAS_R = 7,          // Reliable to PAS
        ONE_UNRELIABLE = 8, // Send to one client, but don't put in reliable stream, put in unreliable datagram (
                            // could be dropped )
        SPEC = 9,           // Sends to all spectator proxies
    };

    class IEngineFuncs : public ISPModInterface
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
        std::string_view getName() const override
        {
            return "IEngineFuncs";
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
         * @noreturn
         */
        virtual void changeLevel(std::string_view level) const = 0;

        /**
         * @brief Executes server's command.
         *
         * @note Executes command in the next frame. To get the result of the command use serverExecute() afterwards.
         *
         * @param cmd          Command to execute.
         *
         * @noreturn
         */
        virtual void serverCommand(std::string_view cmd) const = 0;

        /**
         * @brief Executes previously buffered server's command.
         *
         * @noreturn
         */
        virtual void serverExecute() const = 0;

        /**
         * @brief Registers server's command.
         *
         * @param cmd          Command to register.
         *
         * @noreturn
         */
        virtual void registerSrvCommand(std::string_view cmd) const = 0;

        // TODO: Describe funcs
        virtual void messageBegin(MessageDest msgDest,
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

        virtual ~IEngineFuncs() = default;
    };

    class IEngineFuncsHooked : public ISPModInterface
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
        std::string_view getName() const override
        {
            return "IEngineFuncsHooked";
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
         * @param msg          Message to print to the server console.
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
         * @noreturn
         */
        virtual void changeLevel(std::string_view level) const = 0;

        /**
         * @brief Buffers server's command.
         *
         * @note To get the result of the command use serverExecute() afterwards.
         *
         * @param cmd          Command to execute.
         *
         * @noreturn
         */
        virtual void serverCommand(std::string_view cmd) const = 0;

        /**
         * @brief Executes previously buffered server's command.
         *
         * @noreturn
         */
        virtual void serverExecute() const = 0;

        /**
         * @brief Registers server's command.
         *
         * @param cmd          Command to register.
         *
         * @noreturn
         */
        virtual void registerSrvCommand(std::string_view cmd) const = 0;

        // TODO: Describe funcs
        virtual void messageBegin(MessageDest msgDest,
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

        virtual ~IEngineFuncsHooked() = default;
    };

    class IEngineGlobals : public ISPModInterface
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
        std::string_view getName() const override
        {
            return "IEngineGlobals";
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
         * @brief Returns current game time.
         *
         * @return Game time.
         */
        virtual float getTime() const = 0;

        virtual std::string_view getMapName() const = 0;

        virtual ~IEngineGlobals() = default;
    };
} // namespace SPMod
