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

void Logger::OnDebugSpew(const char *msg,
                            ...)
{
    char debugMsg[512];
    va_list paramsList;

    va_start(paramsList, msg);
    std::vsnprintf(debugMsg, sizeof(debugMsg), msg, paramsList);
    va_end(paramsList);

    LogConsoleCore("[SPMOD] ", debugMsg);
}

void Logger::ReportError(const SourcePawn::IErrorReport &report,
                            SourcePawn::IFrameIterator &iter)
{
    auto *spErrorMsg = gSPGlobal->getSPEnvironment()->APIv2()->GetErrorString(report.Code());
    auto getPluginIdentity = [](SourcePawn::IPluginContext *ctx)
    {
        char *pluginIdentity;
        ctx->GetKey(1, reinterpret_cast<void **>(&pluginIdentity));
        return pluginIdentity;
    };

    LogErrorCore("Run time error ", report.Code(), ": ", spErrorMsg);
    LogErrorCore("Error: ", report.Message());

    if (report.Blame() || report.Context())
    {
        LogErrorCore("Blaming:");

        if (report.Blame())
            LogErrorCore("   Function: ", report.Blame()->DebugName());

        if (report.Context())
            LogErrorCore("   Plugin: ", getPluginIdentity(report.Context()));
    }

    if (!iter.Done())
        LogErrorCore("Stack trace:");

    std::size_t entryPos = 0;
    while (!iter.Done())
    {
        if (iter.IsInternalFrame())
        {
            iter.Next();
            continue;
        }

        auto *funcName = iter.FunctionName();
        if (!funcName)
            funcName = "???";

        if (iter.IsScriptedFrame())
        {
            const char *pluginIdentity;

            if (auto *pluginCtx = iter.Context(); pluginCtx)
                pluginIdentity = getPluginIdentity(pluginCtx);
            else
                pluginIdentity = "???";

            LogErrorCore("   [", entryPos, "] ", pluginIdentity, "::",
                                                                    funcName,
                                                                    " (line ",
                                                                    iter.LineNumber(),
                                                                    ")");
        }
        else if(iter.IsNativeFrame())
            LogErrorCore("   [", entryPos, "] ", funcName);

        ++entryPos;
        iter.Next();
    }
}

void Logger::resetErrorState()
{
    m_alreadyReportedError = false;
}

void Logger::_writeErrorToFile(std::string_view errormsg)
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
    std::strftime(fileName, sizeof(fileName), "error_%Y%m%d.log", &convertedTime);

    std::fstream errorFile(gSPGlobal->getLogsDirCore() / fileName, fFlags::out | fFlags::app);

    if (!m_alreadyReportedError)
    {
        errorFile << logDateTime << "Start of error session.\n";
        errorFile << logDateTime << "Info (map ";

        if (STRING(gpGlobals->mapname))
            errorFile << STRING(gpGlobals->mapname);
        else
            errorFile << "<unknown>";

        errorFile << ") (CRC ";

        if (gRehldsServerData)
            errorFile << gRehldsServerData->GetWorldmapCrc();
        else
            errorFile << "<unknown>";

        errorFile << ")\n";
        m_alreadyReportedError = true;
    }

    errorFile << logDateTime << errormsg;
}
