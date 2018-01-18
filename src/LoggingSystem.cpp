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

    LOG_CONSOLE(PLID, "[SPMOD] %s", debugMsg);
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

    LogErrorCore("[SPMOD] Run time error ", report.Code(), ": ", spErrorMsg);
    LogErrorCore("[SPMOD] Error: ", report.Message());

    if (report.Blame() || report.Context())
    {
        LogErrorCore("[SPMOD] Blaming:");

        if (report.Blame())
            LogErrorCore("[SPMOD]    Function: ", report.Blame()->DebugName());

        if (report.Context())
            LogErrorCore("[SPMOD]    Plugin: ", getPluginIdentity(report.Context()));
    }

    if (!iter.Done())
        LogErrorCore("[SPMOD] Stack trace:");

    size_t entryPos = 0;
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

            LogErrorCore("[SPMOD]    [", entryPos, "] ", pluginIdentity, "::",
                                                                    funcName,
                                                                    "(line ",
                                                                    iter.LineNumber(),
                                                                    ")");
        }
        else if(iter.IsNativeFrame())
            LogErrorCore("[SPMOD]    [", entryPos, "] ", funcName);

        ++entryPos;
        iter.Next();
    }
}

void Logger::_writeErrorToFile(std::string_view errormsg)
{
    using fFlags = std::ios_base;

    time_t currentTime;
    time(&currentTime);
    tm *convertedTime = std::localtime(&currentTime);

    char logDateTime[64], fileName[256];
    std::strftime(logDateTime, sizeof(logDateTime), "%Y/%m/%d - %H:%M:%S: ", convertedTime);
    std::strftime(fileName, sizeof(fileName), "error_%Y%m%d.log", convertedTime);

    std::fstream errorFile(gSPGlobal->getLogsDirCore() / fileName, fFlags::out | fFlags::app);

    if (!m_alreadyReportedError)
    {
        errorFile << logDateTime << "Start of error session.\n";
        errorFile << logDateTime  << "Info (map " << STRING(gpGlobals->mapname)
                                                    << ") (CRC "
                                                    << gRehldsServerData->GetWorldmapCrc()
                                                    << ")\n";
        m_alreadyReportedError = true;
    }

    errorFile << logDateTime << errormsg;
}
