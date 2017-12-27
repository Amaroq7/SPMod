/*  PyMod - Python Scripting Engine for Half-Life
 *  Copyright (C) 2018  PyMod Development Team
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

#include <pymod.hpp>

meta_globals_t *gpMetaGlobals;
gamedll_funcs_t *gpGamedllFuncs;
mutil_funcs_t *gpMetaUtilFuncs;

plugin_info_t gPluginInfo =
{
    META_INTERFACE_VERSION,
    "PyMod",
    gPyModVersion,
    __DATE__,
    gPyModAuthor,
    "https://github.com/Amaroq7/PyMod",
    "PYMOD",
    PT_STARTUP,
    PT_ANYTIME
};

static void printInfo()
{
    std::stringstream msg;
    msg << '\n';
    msg << "   PyMod Copyright (c) 2018 " << gPyModAuthor << '\n';
    msg << "   This program comes with ABSOLUTELY NO WARRANTY; for details type `pymod gpl'." << '\n';
    msg << "   This is free software, and you are welcome to redistribute it" << '\n';
    msg << "   under certain conditions; type `pymod gpl' for details." << '\n';
    msg << '\n';
    msg << "PyMod " << gPyModVersion << ", API " << PYMOD_API_VERSION << '\n';
    msg << "PyMod build: " << __TIME__ << " " << __DATE__ << '\n';
    SERVER_PRINT(msg.str().c_str());
}

static void InitializePython()
{
    auto &&homeDir = gPyGlobal->getHome();
    fs::path pythonDir(homeDir);
    pythonDir /= "pylibraries:";
    pythonDir += homeDir;
    pythonDir /= "pymodules";

    //TODO: make equivalent for windows
    setenv("PYTHONHOME", pythonDir.c_str(), 1);

    Py_SetPath(pythonDir.wstring().c_str());

    PyImport_AppendInittab("core", PyInit_core);
    Py_InitializeEx(0);
    PyImport_ImportModule("core");
}

void pyModInfoCommand()
{
    // Print out available commands
    if (CMD_ARGC() == 1)
    {
        std::stringstream msg;

        msg << '\n';
        msg << "Usage: pymod [command] [args]" << '\n';
        msg << "Command:" << '\n';
        msg << "plugins - displays currently loaded plugins" << '\n';
        msg << "gpl - displays pymod license" << '\n';

        SERVER_PRINT(msg.str().c_str());
    }
    else
    {
        std::string arg(CMD_ARGV(1));
        if (arg == "plugins")
        {
            auto pluginManager = static_cast<PluginMngr *>(gPyGlobal->getPluginManager());
            if (!pluginManager)
                return;

            for (auto entry : pluginManager->getPluginsList())
            {
                LOG_CONSOLE(PLID, "[%3d] %-15.15s %-11.10s %-11.15s %-11.20s\n",
                                    entry.first,
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
            msg << "   PyMod - Python Scripting Engine for Half-Life\n";
            msg << "   Copyright (C) 2018  PyMod Development Team\n";
            msg << '\n';
            msg << "   This program is free software: you can redistribute it and/or modify\n";
            msg << "   it under the terms of the GNU General Public License as published by\n";
            msg << "   the Free Software Foundation, either version 3 of the License, or\n";
            msg << "   (at your option) any later version.\n";
            msg << '\n';
            msg << "   This program is distributed in the hop`e that it will be useful,\n";
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

C_DLLEXPORT int Meta_Query(char *interfaceVersion,
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

C_DLLEXPORT int Meta_Attach(PLUG_LOADTIME now,
                            META_FUNCTIONS *pFunctionTable,
                            meta_globals_t *pMGlobals,
                            gamedll_funcs_t *pGamedllFuncs)
{
    gpMetaGlobals = pMGlobals;
    gpGamedllFuncs = pGamedllFuncs;

    memcpy(pFunctionTable, &gMetaFunctionTable, sizeof(META_FUNCTIONS));

    gPyGlobal = std::make_unique<PyGlobal>(GET_PLUGIN_PATH(PLID));

    InitializePython();
    printInfo();

    gPyGlobal->initializePluginManager();
    gPyGlobal->setModName(GET_GAME_INFO(PLID, GINFO_NAME));

    REG_SVR_COMMAND("pymod", pyModInfoCommand);

    return 1;
}

C_DLLEXPORT int Meta_Detach(PLUG_LOADTIME now, PL_UNLOAD_REASON reason)
{
    Py_FinalizeEx();

    return 1;
}
