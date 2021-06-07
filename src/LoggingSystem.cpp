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

#include "LoggingSystem.hpp"
#include "SPGlobal.hpp"

Logger *LoggerMngr::getLogger(std::string_view prefix)
{
    return m_loggers.try_emplace(prefix.data(), std::make_unique<Logger>(prefix)).first->second.get();
}

Logger::Logger(std::string_view prefix) : m_prefix(prefix) {}
Logger::Logger(std::string_view prefix, LogLevel logLevel) : m_prefix(prefix), m_logLevel(logLevel) {}

void Logger::setFilename(std::string_view filename)
{
    m_filename = filename;
}

void Logger::logToConsole(LogLevel level, std::string_view msg) const
{
    if (level < m_logLevel)
        return;

    logToConsoleInternal(level, msg);
}

void Logger::logToFile(LogLevel level, std::string_view msg) const
{
    if (level < m_logLevel)
        return;

    if (m_filename.empty())
        return;

    logToFileInternal(level, msg);
}

void Logger::logToBoth(LogLevel level, std::string_view msg) const
{
    if (level < m_logLevel)
        return;

    logToBothInternal(level, msg);
}

void Logger::sendMsgToConsole(std::string_view msg) const
{
    sendMsgToConsoleInternal(msg);
}

void Logger::setLogLevel(LogLevel level)
{
    m_logLevel = level;
}

LogLevel Logger::getLogLevel() const
{
    return m_logLevel;
}

void Logger::_writeToFile(std::string_view msg) const
{
    using fFlags = std::ios_base;

    time_t currentTime;
    time(&currentTime);
    tm convertedTime;

#if defined __STDC_LIB_EXT1__ || defined SP_MSVC
    #if defined SP_MSVC
    localtime_s(&convertedTime, &currentTime);
    #else
    localtime_s(&currentTime, &convertedTime);
    #endif
#else
    convertedTime = *std::localtime(&currentTime);
#endif

    char logDateTime[64], fileName[256];
    std::strftime(logDateTime, sizeof(logDateTime), "%Y/%m/%d - %H:%M:%S: ", &convertedTime);
    std::strftime(fileName, sizeof(fileName), "logs_%Y%m%d.log", &convertedTime);

    std::fstream logFile(gSPGlobal->getPath(DirType::Logs) / fileName, fFlags::out | fFlags::app | fFlags::ate);

    if (!logFile.tellg())
    {
        logFile << logDateTime << "Start of error session.\n";
        logFile << logDateTime << "Info (map ";
        logFile << gEngine->getMapName();
        logFile << ") (CRC ";
        logFile << std::to_string(gEngine->getWorldmapCrc());
        logFile << ")\n";
    }

    logFile << logDateTime << msg;
}
