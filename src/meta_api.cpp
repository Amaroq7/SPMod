/*
 *  Copyright (C) 2018-2020 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
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

#include "spmod.hpp"
#include "CompilationUtils.hpp"

meta_globals_t *gpMetaGlobals;
gamedll_funcs_t *gpGamedllFuncs;
mutil_funcs_t *gpMetaUtilFuncs;

enginefuncs_t *gpEngineFuncs;

plugin_info_t Plugin_info = {
    META_INTERFACE_VERSION, "SPMod",    gSPModVersion, __DATE__, gSPModAuthor, "https://github.com/Amaroq7/SPMod",
    gSPModLoggerName,       PT_STARTUP, PT_ANYTIME};

C_DLLEXPORT int
    Meta_Query(char *interfaceVersion [[maybe_unused]], plugin_info_t **plinfo, mutil_funcs_t *pMetaUtilFuncs)
{
    *plinfo = &Plugin_info;
    gpMetaUtilFuncs = pMetaUtilFuncs;

    return 1;
}

META_FUNCTIONS gMetaFunctionTable = {nullptr,
                                     nullptr,
                                     GetEntityAPI2,
                                     GetEntityAPI2_Post,
                                     GetNewDLLFunctions,
                                     GetNewDLLFunctions_Post,
                                     GetEngineFunctions,
                                     GetEngineFunctions_Post};

C_DLLEXPORT int Meta_Attach(PLUG_LOADTIME now [[maybe_unused]],
                            META_FUNCTIONS *pFunctionTable,
                            meta_globals_t *pMGlobals,
                            gamedll_funcs_t *pGamedllFuncs)
{
    gpMetaGlobals = pMGlobals;
    gpGamedllFuncs = pGamedllFuncs;

    try
    {
        gSPGlobal = std::make_unique<SPGlobal>(GET_PLUGIN_PATH(PLID));
    }
    catch (const std::exception &e)
    {
        // Failed to initialize gSPGlobal, we gotta use this util func
        LOG_ERROR(PLID, "%s[%s] %s%s%s", CNSL_BOLD, gSPModLoggerName, CNSL_RED, e.what(), CNSL_RESET);
        return 0;
    }

    auto logger = gSPGlobal->getLoggerManager()->getLogger(gSPModLoggerName);

    logger->sendMsgToConsoleInternal(
        "\n   SPMod version ", gSPModVersion, " Copyright (c) 2018-", gCompilationYear, " ", gSPModAuthor,
        "\n   This program comes with ABSOLUTELY NO WARRANTY; for details type `spmod gpl' \
\n   This is free software, and you are welcome to redistribute it\
\n   under certain conditions; type `spmod gpl' for details.\n");

    if (!initRehldsApi())
    {
        logger->logToBothInternal(LogLevel::Error, CNSL_RED, "SPMod requires to have ReHLDS installed");
        return 0;
    }

    logger->sendMsgToConsoleInternal(CNSL_BOLD, CNSL_GREEN, "SPMod ", CNSL_BLUE, "v", gSPModVersion, CNSL_GREEN,
                                     " has been successfully loaded.\n");

    logger->sendMsgToConsoleInternal(CNSL_LBLUE, "SPMod API: ", CNSL_RESET, CNSL_LGREEN, "v", ISPGlobal::MAJOR_VERSION,
                                     ".", ISPGlobal::MINOR_VERSION);
    logger->sendMsgToConsoleInternal(CNSL_LBLUE, "SPMod build: ", CNSL_RESET, CNSL_LGREEN, gCompilationTime, " ",
                                     gCompilationDate);
    logger->sendMsgToConsoleInternal(CNSL_LBLUE, "SPMod from: ", CNSL_RESET, CNSL_LGREEN, APP_COMMIT_URL,
                                     APP_COMMIT_SHA, '\n');

    GET_HOOK_TABLES(PLID, &gpEngineFuncs, nullptr, nullptr);
    memcpy(pFunctionTable, &gMetaFunctionTable, sizeof(META_FUNCTIONS));

    return 1;
}

C_DLLEXPORT int Meta_Detach(PLUG_LOADTIME now [[maybe_unused]], PL_UNLOAD_REASON reason [[maybe_unused]])
{
    auto fwdMngr = gSPGlobal->getForwardManager();
    fwdMngr->getForward(ForwardMngr::FWD_PLUGIN_END)->execFunc(nullptr);
    fwdMngr->clearForwards();

    gSPGlobal->getTimerManager()->clearTimers();
    gSPGlobal->getCommandManager()->clearCommands();
    gSPGlobal->getCvarManager()->clearCvars();
    gSPGlobal->getMenuManager()->clearMenus();
    gSPGlobal->getMessageManager()->clearMessages();
    gSPGlobal->getNativeProxy()->clearNatives();

    gSPGlobal->unloadExts();
    uninstallRehldsHooks();

    return 1;
}
