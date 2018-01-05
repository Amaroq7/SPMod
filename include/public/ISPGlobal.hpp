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

#include <IPluginMngr.hpp>

// Platform defines
#ifdef _WIN32
    #define SP_WINDOWS
#elif __linux__
    #define SP_LINUX
    #define SP_POSIX
#endif

#ifndef SPMOD_API
    #ifdef SP_POSIX
        #define SPMOD_API	extern "C" __attribute__((visibility("default")))
    #else
        #define SPMOD_API	extern "C" __declspec(dllexport)
    #endif
#endif

namespace SPMod
{
    using sp_api_t = unsigned long;
    constexpr sp_api_t SPMOD_API_VERSION = 0;

    class ISPGlobal
    {
    public:
        virtual const char *getHome() const = 0;
        virtual const char *getModName() const = 0;
        virtual IPluginMngr *getPluginManager() const = 0;
        virtual bool addModule(sp_nativeinfo_t *natives, const char *name, sp_api_t api = SPMOD_API_VERSION) = 0;
        virtual SourcePawn::ISourcePawnEnvironment *getSPEnvironment() const = 0;

    protected:
        virtual ~ISPGlobal() {};
    };

    using fnSPModQuery = int (*)(ISPGlobal *spmodInstance, sp_api_t apiversion);
    SPMOD_API int SPMod_Query(ISPGlobal *spmodInstance, sp_api_t apiversion);
}
