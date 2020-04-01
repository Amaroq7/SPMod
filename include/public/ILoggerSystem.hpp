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

namespace SPMod
{
    class ILogger
    {
    public:
        virtual ~ILogger() = default;

        /**
         * @brief Sets logging file.
         *
         * @note The base path is the logs dir.
         * @note If file doesn't exist it will be created.
         *
         * @param filename        Name of the file to log messages to.
         *
         */
        virtual void setFilename(std::string_view filename) = 0;

        /**
         * @brief Logs to the console.
         *
         * @param level         Level of the log.
         * @param msg           Message to log.
         *
         */
        virtual void logToConsole(LogLevel level, std::string_view msg) const = 0;

        /**
         * @brief Logs to the file.
         *
         * @note The base path is SPMod logs directory.
         * @note If file does not exist, it will be created.
         * @note setFilename() has to be called at least once before executing this function.
         *
         * @param level         Level of the log.
         * @param msg           Message to log.
         *
         */
        virtual void logToFile(LogLevel level, std::string_view msg) const = 0;

        /**
         * @brief Logs to the console and the file.
         *
         * @param level         Level of the log.
         * @param msg           Message to log.
         *
         */
        virtual void logToBoth(LogLevel level, std::string_view msg) const = 0;

        /**
         * @brief Sends message to the console.
         *
         * @param msg           Message to log.
         *
         */
        virtual void sendMsgToConsole(std::string_view msg) const = 0;

        /**
         * @brief Sets log level.
         *
         * @param logLevel      Log level to set.
         *
         */
        virtual void setLogLevel(LogLevel logLevel) = 0;

        /**
         * @brief Gets log level.
         *
         * @return              Currently set log level.
         */
        virtual LogLevel getLogLevel() const = 0;
    };

    class ILoggerMngr : public ISPModInterface
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
        std::string_view getName() const override
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

        virtual ~ILoggerMngr() = default;

        /**
         * @brief Creates a new logger.
         *
         * @note If logger with provided prefix doesn't exist, it will be created.
         *
         * @param prefix          Logger's prefix.
         *
         * @return      Logger's instance.
         */
        virtual ILogger *getLogger(std::string_view prefix) = 0;
    };
} // namespace SPMod
