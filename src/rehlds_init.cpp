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
#include <interface.cpp>

IRehldsApi *gRehldsApi;
const RehldsFuncs_t *gRehldsFuncs;
IRehldsHookchains *gRehldsHookchains;
IRehldsServerStatic *gRehldsServerStatic;

static bool _initRehldsApi(CSysModule *module,
                            std::string *error = nullptr)
{
    if (!module)
    {
        if (error)
            *error = "Failed to locate engine module";

        return false;
    }

    CreateInterfaceFn ifaceFactory = Sys_GetFactory(module);
    if (!ifaceFactory)
    {
        if (error)
            *error = "Failed to locate interface factory in engine module";

        return false;
    }

    int retCode = 0;
    gRehldsApi = reinterpret_cast<IRehldsApi *>(ifaceFactory(VREHLDS_HLDS_API_VERSION, &retCode));
    if (!gRehldsApi)
    {
        if (error)
        {
            *error = "Failed to retrieve rehlds api interface, code ";
            *error += retCode;
        }

        return false;
    }

    auto majorVersion = gRehldsApi->GetMajorVersion();
    auto minorVersion = gRehldsApi->GetMinorVersion();

    if (majorVersion != REHLDS_API_VERSION_MAJOR)
    {
        std::stringstream msg;

        msg << "ReHLDS API Major version mismatch; expected " << REHLDS_API_VERSION_MAJOR;
        msg << ", got " << majorVersion;
        *error = msg.str();

        return false;
    }

    if (minorVersion < REHLDS_API_VERSION_MINOR)
    {
        std::stringstream msg;

        msg << "ReHLDS API minor version mismatch; excpected at least " << REHLDS_API_VERSION_MINOR;
        msg << ", got " << minorVersion;
        *error = msg.str();

        return false;
    }

    gRehldsFuncs = gRehldsApi->GetFuncs();
    gRehldsHookchains = gRehldsApi->GetHookchains();
    gRehldsServerStatic = gRehldsApi->GetServerStatic();

    return true;
}

bool initRehldsApi()
{
    std::string errorMsg;

#ifdef SP_POSIX
    CSysModule *engineModule = Sys_LoadModule("engine_i486.so");
    if (!_initRehldsApi(engineModule, &errorMsg))
    {
        LOG_CONSOLE(PLID, "%s", errorMsg.c_str());
        return false;
    }
#else
    CSysModule *engineModule = Sys_LoadModule("swds.dll");
    if (!_initRehldsApi(engineModule))
    {
        engineModule = Sys_LoadModule("filesystem_stdio.dll");
        if (!_initRehldsApi(engineModule, &errorMsg))
        {
            LOG_CONSOLE(PLID, "%s", errorMsg.c_str());
            return false;
        }
    }
#endif

    return true;
}
