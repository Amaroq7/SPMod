/*
 *  Copyright (C) 2018 SPMod Development Team
 *
 *  This file is part of SPMod.
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

#include "ext.hpp"

SPMod::ISPGlobal *gSPGlobal;
ModuleInterface gModuleInterface;
meta_globals_t *gpMetaGlobals;
gamedll_funcs_t *gpGamedllFuncs;
mutil_funcs_t *gpMetaUtilFuncs;

plugin_info_t Plugin_info =
{
    META_INTERFACE_VERSION,
    "SPMod Meta Test Module",
    "0.0.0",
    __DATE__,
    "SPMod Development Team",
    "https://github.com/Amaroq7/SPMod",
    "SPMOD",
    PT_ANYTIME,
    PT_ANYTIME
};

META_FUNCTIONS gMetaFunctionTable =
{
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr
};

C_DLLEXPORT int Meta_Query(char *interfaceVersion [[maybe_unused]],
                           plugin_info_t **plinfo,
                           mutil_funcs_t *pMetaUtilFuncs)
{
    gpMetaUtilFuncs = pMetaUtilFuncs;
    *plinfo = &Plugin_info;

    LOG_CONSOLE(PLID, "%s %s", __FILE__, __func__);

    return 1;
}

C_DLLEXPORT int Meta_Attach(PLUG_LOADTIME now [[maybe_unused]],
                            META_FUNCTIONS *pFunctionTable,
                            meta_globals_t *pMGlobals,
                            gamedll_funcs_t *pGamedllFuncs)
{
    gpMetaGlobals = pMGlobals;

    memcpy(pFunctionTable, &gMetaFunctionTable, sizeof(META_FUNCTIONS));
    gpGamedllFuncs = pGamedllFuncs;

    LOG_CONSOLE(PLID, "%s %s", __FILE__, __func__);

    return 1;
}

C_DLLEXPORT int Meta_Detach(PLUG_LOADTIME now [[maybe_unused]],
                            PL_UNLOAD_REASON reason [[maybe_unused]])
{
    LOG_CONSOLE(PLID, "%s %s", __FILE__, __func__);

    return 1;
}

SPMOD_API SPMod::ExtQueryValue SPMod_Query(SPMod::ISPGlobal *spmodInstance)
{
    gSPGlobal = spmodInstance;
    gSPGlobal->registerInterface(&gModuleInterface);

    return SPMod::ExtQueryValue::MetaExt;
}

SPMOD_API bool SPMod_Init()
{
    LOG_CONSOLE(PLID, "%s %s", __FILE__, __func__);
    return true;
}

SPMOD_API void SPMod_End()
{
    LOG_CONSOLE(PLID, "%s %s", __FILE__, __func__);
}
