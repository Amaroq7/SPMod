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

#include <cstddef>

namespace SPMod
{
    enum class LogType : uint8_t
    {
        Debug = 0,
        Info,
        Warning,
        Error
    };

    class ILogger
    {
    public:
        /**
         * @brief Sets logging file.
         *
         * @note The base path is the logs dir.
         * @note If file doesn't exist it will be created.
         * 
         * @param filename        Name of the file to log messages to.
         *
         * @noreturn
         */
        virtual void setFilename(const char *filename) = 0;

        /**
         * @brief Logs to the console.
         *
         * @param type          Type of log.
         * @param format        Formatting string.
         * @param ...           Formatting parameters.
         *
         * @noreturn
         */
        virtual void logToConsole(LogType type, const char *format, ...) const = 0;

        /**
         * @brief Logs to the file.
         * 
         * @note The base path is SPMod logs directory.
         * @note If file does not exist, it will be created.
         * @note setFilename() has to be called at least once before executing this function.
         *
         * @param type          Type of log.
         * @param format        Formatting string.
         * @param ...           Formatting parameters.
         *
         * @noreturn
         */
        virtual void logToFile(LogType type, const char *format, ...) const = 0;

        /**
         * @brief Logs to the console and the file.
         * 
         * @param type          Type of log.
         * @param format        Formatting string.
         * @param ...           Formatting parameters.
         *
         * @noreturn
         */
        virtual void logToBoth(LogType type, const char *format, ...) const = 0;

        /**
         * @brief Sends message to the console.
         *
         * @param format        Formatting string.
         * @param ...           Formatting parameters.
         *
         * @noreturn
         */
        virtual void sendMsgToConsole(const char *format, ...) const = 0;

    protected:
        virtual ~ILogger() = default;
    };

    class ILoggerMngr SPMOD_FINAL : public ISPModInterface
    {
    public:
        static constexpr uint16_t MAJOR_VERSION = 0;
        static constexpr uint16_t MINOR_VERSION = 0;

        static constexpr uint32_t VERSION = (MAJOR_VERSION << 16 | MINOR_VERSION);

        /**
         * @brief Gets interface's name.
         *
         * @return              Interface's name.
         */
        const char *getName() const override
        {
            return "ILoggerMngr";
        }

        /**
         * @brief Gets interface's version.
         *
         * @note The first 16 most significant bits represent major version, the rest represent minor version.
         *
         * @return      Interface's version.
         */
        uint32_t getVersion() const override
        {
            return VERSION;
        }

        /**
         * @brief Creates a new logger.
         *
         * @note If logger with provided prefix doesn't exist, it will be created.
         * 
         * @param prefix          Logger's prefix.
         *
         * @return      Logger's instance.
         */
        virtual ILogger *getLogger(const char *prefix) = 0;

    protected:
        virtual ~ILoggerMngr() = default;
    };
}
