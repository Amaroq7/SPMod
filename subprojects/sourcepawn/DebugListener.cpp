/*
 *  Copyright (C) 2018-2020 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SPMod is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with SPMod.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "DebugListener.hpp"
#include "ExtMain.hpp"

#include <ILoggerSystem.hpp>
#include <Common.hpp>

#include <cstdarg>

namespace SPExt
{
    void DebugListener::OnDebugSpew(const char *msg, ...)
    {
        char debugMsg[512];
        va_list paramsList;

        va_start(paramsList, msg);
        std::vsnprintf(debugMsg, sizeof(debugMsg), msg, paramsList);
        va_end(paramsList);

        gSPLogger->logToConsole(SPMod::LogLevel::Debug, debugMsg);
    }

    void DebugListener::ReportError(const SourcePawn::IErrorReport &report, SourcePawn::IFrameIterator &iter)
    {
        using namespace std::string_literals;

        const char *spErrorMsg = gSPAPI->getSPEnvironment()->APIv2()->GetErrorString(report.Code());
        auto getPluginIdentity = [](SourcePawn::IPluginContext *ctx)
        {
            char *pluginIdentity;
            ctx->GetKey(1, reinterpret_cast<void **>(&pluginIdentity));
            return pluginIdentity;
        };
        gSPLogger->logToBoth(SPMod::LogLevel::Error,
                             "Run time error " + std::to_string(report.Code()) + ": " + spErrorMsg);
        gSPLogger->logToBoth(SPMod::LogLevel::Error, "Error: "s + report.Message());

        if (report.Blame() || report.Context())
        {
            gSPLogger->logToBoth(SPMod::LogLevel::Error, "Blaming:");

            if (report.Blame())
                gSPLogger->logToBoth(SPMod::LogLevel::Error, "   Function: "s + report.Blame()->DebugName());

            if (report.Context())
                gSPLogger->logToBoth(SPMod::LogLevel::Error, "   Plugin: "s + getPluginIdentity(report.Context()));
        }

        if (!iter.Done())
            gSPLogger->logToBoth(SPMod::LogLevel::Error, "Stack trace:");

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

                gSPLogger->logToBoth(SPMod::LogLevel::Error, "   [" + std::to_string(entryPos) + "] " + pluginIdentity +
                                                                 "::" + funcName + " (line " +
                                                                 std::to_string(iter.LineNumber()) + ")");
            }
            else if (iter.IsNativeFrame())
                gSPLogger->logToBoth(SPMod::LogLevel::Error, "   [" + std::to_string(entryPos) + "] " + funcName);

            ++entryPos;
            iter.Next();
        }
    }
}