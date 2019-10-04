/*
 *  Copyright (C) 2019 SPMod Development Team
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
        const char *getName() const override
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
        virtual const char *getArg(int arg) const = 0;

        /**
         * @brief Returns current command's arguments.
         *
         * @return Command's arguments.
         */
        virtual const char *getArgs() const = 0;

        /**
         * @brief Returns current command's arguments count.
         *
         * @param arg          Argument to get.
         *
         * @return Command's arguments count.
         */
        virtual int getArgc() const = 0;

        /**
         * @brief Prints message to server's console.
         *
         * @param msg          Message to print to the server's console.
         *
         * @noreturn
         */
        virtual void serverPrint(const char *msg) const = 0;

        /**
         * @brief Precaches model.
         *
         * @param model          Model to precache.
         *
         * @return Precached model's index.
         */
        virtual int precacheModel(const char *model) const = 0;

        /**
         * @brief Precaches sound.
         *
         * @param sound          Sound to precache.
         *
         * @return Precached sound's index.
         */
        virtual int precacheSound(const char *sound) const = 0;

        /**
         * @brief Precaches generic resource.
         *
         * @param generic          Generic resource to precache.
         *
         * @return Precached generic resource's index.
         */
        virtual int precacheGeneric(const char *generic) const = 0;

        /**
         * @brief Changes level.
         *
         * @param generic          Level to change to.
         *
         * @noreturn
         */
        virtual void changeLevel(const char *level) const = 0;

        /**
         * @brief Executes server's command.
         *
         * @note Executes command in the next frame. To get the result of the command use serverExecute() afterwards.
         *
         * @param cmd          Command to execute.
         *
         * @noreturn
         */
        virtual void serverCommand(const char *cmd) const = 0;

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
        virtual void registerSrvCommand(const char *cmd) const = 0;

        // TODO: Describe funcs
        virtual void
            messageBegin(int msgDest, int msgType, const float *pOrigin = nullptr, IEdict *pEdict = nullptr) const = 0;
        virtual void messageEnd() const = 0;
        virtual void writeByte(int byteArg) const = 0;
        virtual void writeChar(int charArg) const = 0;
        virtual void writeShort(int shortArg) const = 0;
        virtual void writeLong(int longArg) const = 0;
        virtual void writeEntity(int entArg) const = 0;
        virtual void writeAngle(float angleArg) const = 0;
        virtual void writeCoord(float coordArg) const = 0;
        virtual void writeString(const char *strArg) const = 0;

    protected:
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
        const char *getName() const override
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
        virtual const char *getArg(int arg) const = 0;

        /**
         * @brief Returns current command's arguments.
         *
         * @return Command's arguments.
         */
        virtual const char *getArgs() const = 0;

        /**
         * @brief Returns current command's arguments count.
         *
         * @param arg          Argument to get.
         *
         * @return Command's arguments count.
         */
        virtual int getArgc() const = 0;

        /**
         * @brief Prints message to server's console.
         *
         * @param msg          Message to print to the server console.
         *
         * @noreturn
         */
        virtual void serverPrint(const char *msg) const = 0;

        /**
         * @brief Precaches model.
         *
         * @param model          Model to precache.
         *
         * @return Precached model's index.
         */
        virtual int precacheModel(const char *model) const = 0;

        /**
         * @brief Precaches sound.
         *
         * @param sound          Sound to precache.
         *
         * @return Precached sound's index.
         */
        virtual int precacheSound(const char *sound) const = 0;

        /**
         * @brief Precaches generic resource.
         *
         * @param generic          Generic resource to precache.
         *
         * @return Precached generic resource's index.
         */
        virtual int precacheGeneric(const char *generic) const = 0;

        /**
         * @brief Changes level.
         *
         * @param generic          Level to change to.
         *
         * @noreturn
         */
        virtual void changeLevel(const char *level) const = 0;

        /**
         * @brief Buffers server's command.
         *
         * @note To get the result of the command use serverExecute() afterwards.
         *
         * @param cmd          Command to execute.
         *
         * @noreturn
         */
        virtual void serverCommand(const char *cmd) const = 0;

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
        virtual void registerSrvCommand(const char *cmd) const = 0;

        // TODO: Describe funcs
        virtual void
            messageBegin(int msgDest, int msgType, const float *pOrigin = nullptr, IEdict *pEdict = nullptr) const = 0;
        virtual void messageEnd() const = 0;
        virtual void writeByte(int byteArg) const = 0;
        virtual void writeChar(int charArg) const = 0;
        virtual void writeShort(int shortArg) const = 0;
        virtual void writeLong(int longArg) const = 0;
        virtual void writeEntity(int entArg) const = 0;
        virtual void writeAngle(float angleArg) const = 0;
        virtual void writeCoord(float coordArg) const = 0;
        virtual void writeString(const char *strArg) const = 0;

    protected:
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
        const char *getName() const override
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

        virtual const char *getMapName() const = 0;

    protected:
        virtual ~IEngineGlobals() = default;
    };
} // namespace SPMod
