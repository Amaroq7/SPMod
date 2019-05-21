/*  SPMod - SourcePawn Scripting Engine for Half-Life
 *  Copyright (C) 2018  SPMod Development Team
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

#include "spmod.hpp"

class LoggerMngr;

class Logger final : public ILogger
{
    friend class LoggerMngr;

public:
    Logger() = delete;
    Logger(const Logger &other) = delete;
    Logger &operator=(const Logger &other) = delete;
    Logger &operator=(Logger &&other) = delete;
    Logger(Logger &&other) = delete;
    ~Logger() = default;

    Logger(std::string_view prefix);

    // ILogger
    void setFilename(const char *filename) override;
    void logToConsole(LogLevel level,
                      const char *format,
                      ...) const override;

    void logToFile(LogLevel level,
                   const char *format,
                   ...) const override;

    void logToBoth(LogLevel level,
                   const char *format,
                   ...) const override;

    void sendMsgToConsole(const char *format,
                          ...) const override;

    void setLogLevel(LogLevel logType) override;
    LogLevel getLogLevel() const override;

    // Logger
    template <typename ...Args>
    void sendMsgToConsoleCore(Args... args) const
    {
        std::stringstream messageToLog;

        (messageToLog << ... << args);
        messageToLog << '\n';

        SERVER_PRINT(messageToLog.str().c_str());
    }

    template <typename ...Args>
    void logToConsoleCore(LogLevel level,
                          Args... args) const
    {
        if (level < m_logLevel)
            return;

        std::stringstream messageToLog;

        messageToLog << '[' << m_prefix << "] ";
        (messageToLog << ... << args);
        messageToLog << '\n';

        SERVER_PRINT(messageToLog.str().c_str());
    }

    template <typename ...Args>
    void logToFileCore(LogLevel level,
                       Args... args) const
    {
        if (level < m_logLevel)
            return;

        if (m_filename.empty())
            return;

        std::stringstream messageToLog;

        messageToLog << '[' << m_prefix << "] ";
        (messageToLog << ... << args);
        messageToLog << '\n';

        _writeToFile(messageToLog.str());
    }

    template <typename ...Args>
    void logToBothCore(LogLevel level,
                       Args... args) const
    {
        if (level < m_logLevel)
            return;

        std::stringstream messageToLog;

        messageToLog << '[' << m_prefix << "] ";
        (messageToLog << ... << args);
        messageToLog << '\n';

        SERVER_PRINT(messageToLog.str().c_str());

        if (m_filename.empty())
            return;

        _writeToFile(messageToLog.str());
    }

private:
    std::string m_prefix;
    std::string m_filename;
    LogLevel m_logLevel;
    void _writeToFile(std::string_view msg) const;
};

class LoggerMngr final : public ILoggerMngr
{
public:
    LoggerMngr() = default;
    LoggerMngr(const LoggerMngr &other) = delete;
    LoggerMngr(LoggerMngr &&other) = default;
    ~LoggerMngr() = default;

    // ILoggerMngr
    ILogger *getLogger(const char *prefix) override;

    // LoggerMngr
    std::shared_ptr<Logger> getLoggerCore(std::string_view prefix);

private:
    std::unordered_map<std::string, std::shared_ptr<Logger>> m_loggers;
};
