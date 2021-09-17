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

#include "AnubisInit.hpp"

#include <fmt/format.h>
#include <fmt/color.h>
#include <fmt/ostream.h>

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
    Logger(std::string_view prefix, LogLevel logLevel);

    // ILogger
    void setFilename(std::string_view filename) final;
    void logToConsole(LogLevel level, std::string_view msg) const final;
    void logToFile(LogLevel level, std::string_view msg) const final;
    void logToBoth(LogLevel level, std::string_view msg) const final;
    void sendMsgToConsole(std::string_view msg) const final;
    void setLogLevel(LogLevel logType) final;
    LogLevel getLogLevel() const final;

    // Logger
    template<typename... t_args>
    void sendMsgToConsole(std::string_view format, t_args... args) const
    {
        gEngine->print(fmt::vformat(format, fmt::make_args_checked<t_args...>(format, args...)).c_str(),
                                     Anubis::FuncCallType::Direct);
    }

    template<typename... t_args>
    void logToConsole(LogLevel level, std::string_view format, t_args... args) const
    {
        if (level < m_logLevel)
            return;

        gEngine->print(fmt::format("[{}] {}", m_prefix, fmt::vformat(format, fmt::make_args_checked<t_args...>(format, args...))).c_str(),
                       Anubis::FuncCallType::Direct);
    }

    template<typename... t_args>
    void logToFile(LogLevel level, std::string_view format, t_args... args) const
    {
        if (level < m_logLevel)
            return;

        if (m_filename.empty())
            return;

        _writeToFile(fmt::format("[{}] {}", m_prefix, fmt::vformat(format, fmt::make_args_checked<t_args...>(format, args...))));
    }

    template<typename... t_args>
    void logToBoth(LogLevel level, std::string_view format, t_args... args) const
    {
        if (level < m_logLevel)
            return;

        std::string message = fmt::format("[{}] {}", m_prefix, fmt::vformat(format, fmt::make_args_checked<t_args...>(format, args...)));
        gEngine->print(message.c_str(), Anubis::FuncCallType::Direct);

        if (m_filename.empty())
            return;

        _writeToFile(message);
    }

private:
    static void _writeToFile(std::string_view msg);

private:
    std::string m_prefix;
    LogLevel m_logLevel;
    std::string m_filename;
};

class LoggerMngr final : public ILoggerMngr
{
public:
    LoggerMngr() = default;
    LoggerMngr(const LoggerMngr &other) = delete;
    LoggerMngr(LoggerMngr &&other) = default;
    ~LoggerMngr() = default;

    // ILoggerMngr
    nstd::observer_ptr<ILogger> getLogger(std::string_view prefix) final;

private:
    std::unordered_map<std::string, std::unique_ptr<Logger>> m_loggers;
};
