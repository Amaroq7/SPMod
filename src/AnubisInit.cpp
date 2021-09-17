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
#include "AnubisInit.hpp"
#include "EngineHooks.hpp"
#include "DLLHooks.hpp"
#include "ReHooks.hpp"

nstd::observer_ptr<Anubis::IAnubis> gAnubisAPI;
nstd::observer_ptr<Anubis::Engine::ILibrary> gEngine;
nstd::observer_ptr<Anubis::Game::ILibrary> gGame;
nstd::observer_ptr<Anubis::ILogger> gLogger;

static std::unique_ptr<AnubisPlugin> gAnubisPlugin;

namespace Anubis
{
    nstd::observer_ptr<IPlugin> Query()
    {
        gAnubisPlugin = std::make_unique<AnubisPlugin>();
        return gAnubisPlugin;
    }

    bool Init(nstd::observer_ptr<IAnubis> api)
    {
        gAnubisAPI = api;
        gGame = gAnubisAPI->getGame(Game::ILibrary::VERSION);
        gEngine = gAnubisAPI->getEngine(Engine::ILibrary::VERSION);

        gLogger = gAnubisAPI->getLogger(ILogger::VERSION);
        gLogger->setLogTag("SPMod");
        gLogger->setFilename("spmod");
#if defined DEBUG
        gLogger->setLogLevel(LogLevel::Debug);
#else
        gLogger->setLogLevel(LogLevel::Info);
#endif

        try
        {
            std::filesystem::path pluginPath = gAnubisPlugin->getPath();
            gSPGlobal = std::make_unique<SPGlobal>(std::move(pluginPath));
        }
        catch (const std::exception &e)
        {
            using namespace fmt::literals;
            fmt::text_style textStyle(fg(fmt::terminal_color::bright_red) | fmt::emphasis::bold);

            gLogger->logMsg(LogDest::Console | LogDest::File, LogLevel::Error, fmt::format(textStyle, "{}", e.what()));
            return false;
        }

        gEngine->print(fmt::format("\n   SPMod version {}  Copyright (c) 2018-{} {}"
                       "\n   This program comes with ABSOLUTELY NO WARRANTY; for details type `spmod gpl'"
                       "\n   This is free software, and you are welcome to redistribute it"
                       "\n   under certain conditions; type `spmod gpl' for details.\n",
                                   gSPModVersion, gCompilationYear, gSPModAuthor), Anubis::FuncCallType::Direct);

        fmt::text_style textStyleGreenBold(fg(fmt::terminal_color::bright_green) | fmt::emphasis::bold);
        fmt::text_style textStyleCyan(fg(fmt::terminal_color::cyan));

        gEngine->print(fmt::format(textStyleGreenBold, "SPMod v{} has been successfully loaded.\n", gSPModVersion), Anubis::FuncCallType::Direct);
        gEngine->print(fmt::format(textStyleCyan, "SPMod API: v{}.{}\n", ISPGlobal::MAJOR_VERSION, ISPGlobal::MINOR_VERSION), Anubis::FuncCallType::Direct);
        gEngine->print(fmt::format(textStyleCyan, "SPMod build: {} {}\n", gCompilationTime, gCompilationDate), Anubis::FuncCallType::Direct);
        gEngine->print(fmt::format(textStyleCyan, "SPMod from: {}{}\n", APP_COMMIT_URL, APP_COMMIT_SHA), Anubis::FuncCallType::Direct);

        installEngineHooks();
        installDLLHooks();
        installReHooks();

        return true;
    }

    void Shutdown()
    {
        gSPGlobal->unloadExts();
        gLogger.reset();
    }
}