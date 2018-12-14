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
    Logger(Logger &&other) = default;
    ~Logger() = default;

    Logger(std::string_view prefix);

    // ILogger
    void setFilename(const char *filename) override;
    void logToConsole(LogType type, const char *format, ...) const override;
    void logToFile(LogType type, const char *format, ...) const override;
    void logToBoth(LogType type, const char *format, ...) const override;
    void sendMsgToConsole(const char *format, ...) const override;

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
    void logToConsoleCore(LogType type, Args... args) const
    {
        /*
        if (type < logLevelToPrint)
            return;
        */

        std::stringstream messageToLog;

        messageToLog << m_prefix << " ";
        (messageToLog << ... << args);
        messageToLog << '\n';

        SERVER_PRINT(messageToLog.str().c_str());
    }

    template <typename ...Args>
    void logToFileCore(LogType type, Args... args) const
    {
        /*
        if (type < logLevelToPrint)
            return;
        */

        if (m_filename.empty())
            return;

        std::stringstream messageToLog;

        messageToLog << m_prefix << " ";
        (messageToLog << ... << args);
        messageToLog << '\n';

        _writeToFile(messageToLog.str());
    }

    template <typename ...Args>
    void logToBothCore(LogType type, Args... args) const
    {
        /*
        if (type < logLevelToPrint)
            return;
        */

        std::stringstream messageToLog;

        messageToLog << m_prefix << " ";
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