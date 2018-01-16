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

#include "spmod.hpp"

meta_globals_t *gpMetaGlobals;
gamedll_funcs_t *gpGamedllFuncs;
mutil_funcs_t *gpMetaUtilFuncs;

std::unique_ptr<SPModModule> gSPModModuleDef;

plugin_info_t gPluginInfo =
{
    META_INTERFACE_VERSION,
    "SPMod",
    gSPModVersion,
    __DATE__,
    gSPModAuthor,
    "https://github.com/Amaroq7/SPMod",
    "SPMOD",
    PT_STARTUP,
    PT_ANYTIME
};

constexpr auto sourcepawnLibrary = "sourcepawn.jit.x86.so";

static void printInfo()
{
    std::stringstream msg;
    msg << '\n';
    msg << "   SPMod Copyright (c) 2018 " << gSPModAuthor << '\n';
    msg << "   This program comes with ABSOLUTELY NO WARRANTY; for details type `spmod gpl'." << '\n';
    msg << "   This is free software, and you are welcome to redistribute it" << '\n';
    msg << "   under certain conditions; type `spmod gpl' for details." << '\n';
    msg << '\n';
    msg << "SPMod " << gSPModVersion << ", API " << SPMOD_API_VERSION << '\n';
    msg << "SPMod build: " << __TIME__ << " " << __DATE__ << '\n';
    SERVER_PRINT(msg.str().c_str());
}

static bool InitializeSourcePawn()
{
    auto homeDir = gSPGlobal->getHome();
    fs::path dllsDir(homeDir);
    dllsDir /= "dlls";
    dllsDir /= sourcepawnLibrary;

#ifdef SP_POSIX
    void *libraryHandle = dlopen(dllsDir.c_str(), RTLD_NOW);
#else
    // TODO: windows
#endif

    if (!libraryHandle)
    {
        LOG_CONSOLE(PLID, "[SPMod] Failed to open SourcePawn library!");
#ifdef SP_POSIX
        LOG_CONSOLE(PLID, "%s", dlerror());
#else
        //TODO: windows
#endif
        return false;
    }

#ifdef SP_POSIX
    auto getFactoryFunc = reinterpret_cast<SourcePawn::GetSourcePawnFactoryFn>
                                (dlsym(libraryHandle, "GetSourcePawnFactory"));
#else
    //TODO: windows
#endif
    if (!getFactoryFunc)
    {
        LOG_CONSOLE(PLID, "[SPMod] Cannot find factory function!");
#ifdef SP_POSIX
        dlclose(libraryHandle);
#else
        //TODO: windows
#endif
        return false;
    }

    SourcePawn::ISourcePawnFactory *SPFactory = getFactoryFunc(SOURCEPAWN_API_VERSION);
    if (!SPFactory)
    {
        LOG_CONSOLE(PLID, "[SPMod] Wrong SourcePawn library version!");
        #ifdef SP_POSIX
        dlclose(libraryHandle);
        #else
        // TODO: windows
        #endif
        return false;
    }

    gSPGlobal->setSPFactory(libraryHandle, SPFactory);
    gSPGlobal->getSPEnvironment()->APIv2()->SetJitEnabled(true);

    return true;
}

void SPModInfoCommand()
{
    // Print out available commands
    if (CMD_ARGC() == 1)
    {
        std::stringstream msg;

        msg << '\n';
        msg << "Usage: spmod [command] [args]" << '\n';
        msg << "Command:" << '\n';
        msg << "plugins - displays currently loaded plugins" << '\n';
        msg << "gpl - displays spmod license" << '\n';

        SERVER_PRINT(msg.str().c_str());
    }
    else
    {
        std::string arg(CMD_ARGV(1));
        if (arg == "plugins")
        {
            for (auto entry : gSPGlobal->getPluginManagerCore()->getPluginsList())
            {
                LOG_CONSOLE(PLID, "[%3d] %-15.15s %-11.10s %-11.20s %-11.20s\n",
                                    entry.second->getId(),
                                    entry.second->getName(),
                                    entry.second->getVersion(),
                                    entry.second->getAuthor(),
                                    entry.second->getFileName());
            }
        }
        else if (arg == "gpl")
        {
            std::stringstream msg;

            msg << '\n';
            msg << "   SPMod - SourcePawn Scripting Engine for Half-Life\n";
            msg << "   Copyright (C) 2018  SPMod Development Team\n";
            msg << '\n';
            msg << "   This program is free software: you can redistribute it and/or modify\n";
            msg << "   it under the terms of the GNU General Public License as published by\n";
            msg << "   the Free Software Foundation, either version 3 of the License, or\n";
            msg << "   (at your option) any later version.\n";
            msg << '\n';
            msg << "   This program is distributed in the hope that it will be useful,\n";
            msg << "   but WITHOUT ANY WARRANTY; without even the implied warranty of\n";
            msg << "   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n";
            msg << "   GNU General Public License for more details.\n";
            msg << '\n';
            msg << "   You should have received a copy of the GNU General Public License\n";
            msg << "   along with this program.  If not, see <https://www.gnu.org/licenses/>.\n";
            msg << '\n';

            SERVER_PRINT(msg.str().c_str());
        }
    }
}

C_DLLEXPORT int Meta_Query(char *interfaceVersion [[maybe_unused]],
                            plugin_info_t **plinfo,
                            mutil_funcs_t *pMetaUtilFuncs)
{

    *plinfo = &gPluginInfo;
    gpMetaUtilFuncs = pMetaUtilFuncs;

    return 1;
}

META_FUNCTIONS gMetaFunctionTable =
{
    nullptr,
    nullptr,
    GetEntityAPI2,
    GetEntityAPI2_Post,
    GetNewDLLFunctions,
    GetNewDLLFunctions_Post,
    GetEngineFunctions,
    GetEngineFunctions_Post
};

C_DLLEXPORT int Meta_Attach(PLUG_LOADTIME now [[maybe_unused]],
                            META_FUNCTIONS *pFunctionTable,
                            meta_globals_t *pMGlobals,
                            gamedll_funcs_t *pGamedllFuncs)
{
    gpMetaGlobals = pMGlobals;
    gpGamedllFuncs = pGamedllFuncs;

    if (!initRehldsApi())
        return 0;

    gSPGlobal = std::make_unique<SPGlobal>(GET_PLUGIN_PATH(PLID));
    gSPGlobal->setModName(GET_GAME_INFO(PLID, GINFO_NAME));
    gSPGlobal->setScriptsDir("scripts");
    gSPGlobal->setLogsDir("logs");

    if (!InitializeSourcePawn())
        return 0;

    gSPModModuleDef = std::make_unique<SPModModule>(gCoreNatives);
    gSPGlobal->addModule(gSPModModuleDef.get());
    gSPGlobal->initPluginManager();
    gSPGlobal->initDefaultsForwards();
    gSPGlobal->getSPEnvironment()->APIv2()->SetDebugListener(gSPGlobal->getLoggerCore().get());

    printInfo();

    REG_SVR_COMMAND("spmod", SPModInfoCommand);

    memcpy(pFunctionTable, &gMetaFunctionTable, sizeof(META_FUNCTIONS));

    return 1;
}

C_DLLEXPORT int Meta_Detach(PLUG_LOADTIME now [[maybe_unused]],
                            PL_UNLOAD_REASON reason [[maybe_unused]])
{
    gSPGlobal->getPluginManagerCore()->detachPlugins();
    gSPGlobal->getSPEnvironment()->Shutdown();

    return 1;
}
