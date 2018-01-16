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

    LOG_CONSOLE(PLID, "[SPMOD] Run time error %i: %s", report.Code(), spErrorMsg);
    LOG_CONSOLE(PLID, "[SPMOD] Error: %s", report.Message());

    if (report.Blame() || report.Context())
    {
        LOG_CONSOLE(PLID, "[SPMOD] Blaming:");

        if (report.Blame())
            LOG_CONSOLE(PLID, "[SPMOD]    Function: %s", report.Blame()->DebugName());

        if (report.Context())
            LOG_CONSOLE(PLID, "[SPMOD]    Plugin: %s", getPluginIdentity(report.Context()));
    }

    if (!iter.Done())
        LOG_CONSOLE(PLID, "[SPMOD] Stack trace:");

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

            LOG_CONSOLE(PLID, "[SPMOD]    [%i] %s::%s (line %i)", entryPos,
                                                                    pluginIdentity,
                                                                    funcName,
                                                                    iter.LineNumber());
        }
        else if(iter.IsNativeFrame())
            LOG_CONSOLE(PLID, "[SPMOD]    [%i] %s", entryPos, funcName);

        ++entryPos;
        iter.Next();
    }
}
