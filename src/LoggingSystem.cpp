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
    Plugin *plugin;
    report.Context()->GetKey(1, reinterpret_cast<void **>(&plugin));

    auto pluginIdentity = plugin->getIndentityString();
    auto *spErrorMsg = gSPGlobal->getSPEnvironment()->APIv2()->GetErrorString(report.Code());
    LOG_CONSOLE(PLID, "[SPMOD] Error encountered: %s", report.Message());
    LOG_CONSOLE(PLID, "[SPMOD] Displaying debug trace (plugin \"%s\")", pluginIdentity.c_str());
    LOG_CONSOLE(PLID, "[SPMOD] Run time error %i: %s", report.Code(), spErrorMsg);

    size_t entryPos = 0;
    while (!iter.Done())
    {
        if (!iter.IsScriptedFrame())
        {
            iter.Next();
            continue;
        }

        auto *pluginCtx = iter.Context();
        if (pluginCtx)
        {
            pluginCtx->GetKey(1, reinterpret_cast<void **>(&plugin));
            pluginIdentity = plugin->getIndentityString();
        }
        else
        {
            pluginIdentity = "???";
        }

        auto *funcName = iter.FunctionName();
        if (!funcName)
            funcName = "???";

        LOG_CONSOLE(PLID, "[SPMOD]    [%i] %s::%s (line %i)", entryPos,
                                                                pluginIdentity.c_str(),
                                                                funcName,
                                                                iter.LineNumber());

        ++entryPos;
        iter.Next();
    }
}
