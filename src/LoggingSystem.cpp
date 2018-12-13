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

#include "LoggingSystem.hpp"

Logger::Logger(std::string_view prefix) : m_prefix(prefix)
{
}

ILogger *LoggerMngr::getLogger(const char *prefix)
{
    return getLoggerCore(prefix).get();
}

std::shared_ptr<Logger> LoggerMngr::getLoggerCore(std::string_view prefix)
{
    return m_loggers.try_emplace(prefix.data(), std::make_shared<Logger>(prefix)).first->second;
}

void Logger::setFilename(const char *filename)
{
    m_filename = filename;
}

void Logger::logToConsole(LogType type, const char *format, ...) const
{
    /*
        if (type < logLevelToPrint)
            return;
    */

    char logMsg[512];
    va_list paramsList;

    va_start(paramsList, format);
    std::vsnprintf(logMsg, sizeof(logMsg), format, paramsList);
    va_end(paramsList);

    logToConsoleCore(type, logMsg);
}

void Logger::logToFile(LogType type, const char *format, ...) const
{
    /*
        if (type < logLevelToPrint)
            return;
    */

    if (m_filename.empty())
        return;

    char logMsg[512];
    va_list paramsList;

    va_start(paramsList, format);
    std::vsnprintf(logMsg, sizeof(logMsg), format, paramsList);
    va_end(paramsList);

    logToFileCore(type, logMsg);
}

void Logger::logToBoth(LogType type, const char *format, ...) const
{
    /*
        if (type < logLevelToPrint)
            return;
    */

    char logMsg[512];
    va_list paramsList;

    va_start(paramsList, format);
    std::vsnprintf(logMsg, sizeof(logMsg), format, paramsList);
    va_end(paramsList);

    logToBothCore(type, logMsg);
}

void Logger::sendMsgToConsole(const char *format, ...) const
{
    char logMsg[512];
    va_list paramsList;

    va_start(paramsList, format);
    std::vsnprintf(logMsg, sizeof(logMsg), format, paramsList);
    va_end(paramsList);

    sendMsgToConsoleCore(logMsg);
}

void Logger::resetState()
{
    m_alreadyReported = false;
}

void Logger::_writeToFile(std::string_view msg)
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

    std::fstream logFile(gSPGlobal->getPathCore(DirType::Logs) / fileName, fFlags::out | fFlags::app);

    if (!m_alreadyReported)
    {
        logFile << logDateTime << "Start of error session.\n";
        logFile << logDateTime << "Info (map ";

        if (STRING(gpGlobals->mapname))
            logFile << STRING(gpGlobals->mapname);
        else
            logFile << "<unknown>";

        logFile << ") (CRC ";

        if (gRehldsServerData)
            logFile << gRehldsServerData->GetWorldmapCrc();
        else
            logFile << "<unknown>";

        logFile << ")\n";
        m_alreadyReported = true;
    }

    logFile << logDateTime << msg;
}
