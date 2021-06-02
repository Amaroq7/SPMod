/*
 *  Copyright (C) 2020-2021 SPMod Development Team

 *  This file is part of SPMod.

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

#include "SPGlobal.hpp"
#include "MetaInit.hpp"
#include "EngineHooks.hpp"
#include "DLLHooks.hpp"

MetaPlugin metaPlugin;

Metamod::IMetamod *gMetaAPI;
Metamod::Engine::ILibrary *gEngine;
Metamod::Game::ILibrary *gGame;

C_DLLEXPORT Metamod::IPlugin *Metamod::MetaQuery()
{
    return &metaPlugin;
}

C_DLLEXPORT bool Metamod::MetaInit(Metamod::IMetamod *api)
{
    using namespace std::string_literals;
    gMetaAPI = api;
    gEngine = api->getEngine();
    gGame = api->getGame();

    try
    {
        gSPGlobal = std::make_unique<SPGlobal>(metamod->getFuncs()->getPluginPath());
    }
    catch (const std::exception &e)
    {
        api->logMsg(&metaPlugin, Metamod::LogLevel::Error, Metamod::LogDest::Console | Metamod::LogDest::File,
            ""s + CNSL_BOLD + gSPModLoggerName + CNSL_RED + e.what() + CNSL_RESET
        );
        return false;
    }

    auto logger = gSPGlobal->getLoggerManager()->getLogger(gSPModLoggerName);

    logger->sendMsgToConsoleInternal(
        "\n   SPMod version ", gSPModVersion, " Copyright (c) 2018-", gCompilationYear, " ", gSPModAuthor,
        "\n   This program comes with ABSOLUTELY NO WARRANTY; for details type `spmod gpl' \
\n   This is free software, and you are welcome to redistribute it\
\n   under certain conditions; type `spmod gpl' for details.\n");

    logger->sendMsgToConsoleInternal(CNSL_BOLD, CNSL_GREEN, "SPMod ", CNSL_BLUE, "v", gSPModVersion, CNSL_GREEN,
                                     " has been successfully loaded.\n");

    logger->sendMsgToConsoleInternal(CNSL_LBLUE, "SPMod API: ", CNSL_RESET, CNSL_LGREEN, "v", ISPGlobal::MAJOR_VERSION,
                                     ".", ISPGlobal::MINOR_VERSION);
    logger->sendMsgToConsoleInternal(CNSL_LBLUE, "SPMod build: ", CNSL_RESET, CNSL_LGREEN, gCompilationTime, " ",
                                     gCompilationDate);
    logger->sendMsgToConsoleInternal(CNSL_LBLUE, "SPMod from: ", CNSL_RESET, CNSL_LGREEN, APP_COMMIT_URL,
                                     APP_COMMIT_SHA, '\n');

    installEngineHooks();
    installDLLHooks();

    return true;
}

C_DLLEXPORT void Metamod::MetaShutdown()
{
    gSPGlobal->unloadExts();
}