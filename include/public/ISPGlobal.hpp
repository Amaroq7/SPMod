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

#include <IHelpers.hpp>

#ifdef SP_CLANG
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wnon-virtual-dtor"
    #pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined SP_GCC
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
    #pragma GCC diagnostic ignored "-Wunused-parameter"
#elif defined SP_MSVC
    #pragma warning(push)
    // Unreferenced formal parameter
    #pragma warning(disable : 4100)
#endif
#include <sp_vm_api.h>
#ifdef SP_CLANG
    #pragma clang diagnostic pop
#elif defined SP_GCC
    #pragma GCC diagnostic pop
#elif defined SP_MSVC
    #pragma warning(pop)
#endif

#include <IForwardSystem.hpp>
#include <IPluginSystem.hpp>

namespace SPMod
{
    using sp_api_t = unsigned long;
    constexpr sp_api_t SPMOD_API_VERSION = 0;

    class IModuleInterface;

    class ISPGlobal SPMOD_FINAL
    {
    public:
        virtual const char *getHome() const = 0;
        virtual const char *getModName() const = 0;
        virtual IPluginMngr *getPluginManager() const = 0;
        virtual bool addModule(IModuleInterface *interface) = 0;
        virtual IForwardMngr *getForwardManager() const = 0;
        virtual SourcePawn::ISourcePawnEnvironment *getSPEnvironment() const = 0;

    protected:
        virtual ~ISPGlobal() {};
    };

    using fnSPModQuery = int (*)(ISPGlobal *spmodInstance);
    SPMOD_API int SPMod_Query(ISPGlobal *spmodInstance);

    class IModuleInterface
    {
    public:
        virtual sp_api_t getInterfaceVersion() const
        {
            return SPMOD_API_VERSION;
        }
        virtual const sp_nativeinfo_t *getNatives() const
        {
            return m_natives;
        }
        virtual size_t getNativesNum() const
        {
            return m_nativesNum;
        }
        virtual const char *getName() const = 0;

    protected:
        virtual ~IModuleInterface() { }

        sp_nativeinfo_t *m_natives;
        size_t m_nativesNum;
    };
}
