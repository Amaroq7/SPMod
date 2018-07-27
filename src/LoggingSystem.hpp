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

#ifdef SP_CLANG
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#elif defined SP_GCC
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

class Logger final : public SourcePawn::IDebugListener
{
public:
    Logger() = default;
    ~Logger() = default;

    // IDebugListener
    void OnDebugSpew(const char *msg,
                        ...) override;

    void ReportError(const SourcePawn::IErrorReport &report,
                        SourcePawn::IFrameIterator &iter) override;

    // Logger
    template <typename ...Args>
    void LogConsoleCore(Args... args)
    {
        std::stringstream messageToLog;

        (messageToLog << ... << args);
        messageToLog << '\n';

        SERVER_PRINT(messageToLog.str().c_str());
    }

    template <typename ...Args>
    void LogMessageCore(Args... args)
    {
        std::stringstream messageToLog;

        messageToLog << "[SPMOD] ";
        (messageToLog << ... << args);
        messageToLog << '\n';

        // TODO: logging to file
        SERVER_PRINT(messageToLog.str().c_str());
    }

    template <typename ...Args>
    void LogErrorCore(Args... args)
    {
        std::stringstream errorToLog;

        errorToLog << "[SPMOD] ";
        (errorToLog << ... << args);
        errorToLog << '\n';

        _writeErrorToFile(errorToLog.str());

        SERVER_PRINT(errorToLog.str().c_str());
    }

    void resetErrorState();

private:
    void _writeErrorToFile(std::string_view errormsg);
    bool m_alreadyReportedError;
};

#ifdef SP_CLANG
    #pragma clang diagnostic pop
#elif defined SP_GCC
    #pragma GCC diagnostic pop
#endif
