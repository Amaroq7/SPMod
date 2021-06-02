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

#include <Common.hpp>
#include <ILoggerSystem.hpp>

#include "MetaInit.hpp"

using namespace SPMod;

class Logger final : public ILogger
{
public:
    Logger() = delete;
    Logger(const Logger &other) = delete;
    Logger &operator=(const Logger &other) = delete;
    Logger &operator=(Logger &&other) = delete;
    Logger(Logger &&other) = delete;
    ~Logger() override = default;

    explicit Logger(std::string_view prefix);

    // ILogger
    void setFilename(std::string_view filename) override;
    void logToConsole(LogLevel level, std::string_view msg) const override;

    void logToFile(LogLevel level, std::string_view msg) const override;

    void logToBoth(LogLevel level, std::string_view msg) const override;

    void sendMsgToConsole(std::string_view msg) const override;

    void setLogLevel(LogLevel logType) override;
    LogLevel getLogLevel() const override;

    // Logger
    template<typename... Args>
    void sendMsgToConsoleInternal(Args... args) const
    {
        std::stringstream messageToLog;

        (messageToLog << ... << args);
        messageToLog << '\n' << CNSL_RESET;

        gMetaAPI->getEngine()->print(messageToLog.str().c_str(), Metamod::FuncCallType::Direct);
    }

    template<typename... Args>
    void logToConsoleInternal(LogLevel level, Args... args) const
    {
        if (level < m_logLevel)
            return;

        std::stringstream messageToLog;

        messageToLog << '[' << m_prefix << "] ";
        (messageToLog << ... << args);
        messageToLog << '\n' << CNSL_RESET;

        gMetaAPI->getEngine()->print(messageToLog.str().c_str(), Metamod::FuncCallType::Direct);
    }

    template<typename... Args>
    void logToFileInternal(LogLevel level, Args... args) const
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

    template<typename... Args>
    void logToBothInternal(LogLevel level, Args... args) const
    {
        if (level < m_logLevel)
            return;

        std::stringstream messageToLog;

        messageToLog << '[' << m_prefix << "] ";
        (messageToLog << ... << args);
        messageToLog << '\n';

        std::string message = messageToLog.str();

        gMetaAPI->getEngine()->print(message.c_str(), Metamod::FuncCallType::Direct);

        if (m_filename.empty())
            return;

        _writeToFile(message);
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
    Logger *getLogger(std::string_view prefix) override;

private:
    std::unordered_map<std::string, std::unique_ptr<Logger>> m_loggers;
};
