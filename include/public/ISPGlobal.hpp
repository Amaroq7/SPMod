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

#include <cstddef>
#include <IHelpers.hpp>

#ifdef SP_CLANG
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wnon-virtual-dtor"
    #pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined SP_GCC
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
    #pragma GCC diagnostic ignored "-Wunused-parameter"
    #pragma GCC diagnostic ignored "-Wpedantic"
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

#include <IInterface.hpp>
#include <IForwardSystem.hpp>
#include <IPluginSystem.hpp>
#include <ICvarSystem.hpp>
#include <ITimerSystem.hpp>
#include <IMenuSystem.hpp>
#include <IUtilsSystem.hpp>
#include <IPlayerSystem.hpp>

namespace SPMod
{
    class ISPGlobal SPMOD_FINAL : public ISPModInterface
    {
    public:
        static constexpr uint16_t MAJOR_VERSION = 0;
        static constexpr uint16_t MINOR_VERSION = 0;

        static constexpr uint32_t VERSION = (MAJOR_VERSION << 16 | MINOR_VERSION);

        /**
         * @brief Gets interface's name.
         *
         * @return      Interface's name.
         */
        const char *getName() const override
        {
            return "ISPGlobal";
        }

        /**
         * @brief Gets interface's version.
         *
         * @note The first 16 most significant bits represent major version, the rest represent minor version.
         *
         * @return      Interface's version.
         */
        uint32_t getVersion() const override
        {
            return VERSION;
        }

        /**
         * @brief Returns home dir of SPMod.
         *
         * @return        Home dir.
         */
        virtual const char *getHomeDir() const = 0;

        /**
         * @brief Returns name of the mod.
         *
         * @return        Mod name.
         */
        virtual const char *getModName() const = 0;

        /**
         * @brief Returns SPMod plugin manager.
         *
         * @return        Plugin manager.
         */
        virtual IPluginMngr *getPluginManager() const = 0;

        /**
         * @brief Returns SPMod forward manager.
         *
         * @return        Forward manager.
         */
        virtual IForwardMngr *getForwardManager() const = 0;

        /**
        * @brief Returns SPMod cvar manager.
        *
        * @return                cvar manager.
        */
        virtual ICvarMngr *getCvarManager() const = 0;

        /**
         * @brief Return SPMod timer manager.
         *
         * @return              Timer manager.
         */
        virtual ITimerMngr *getTimerManager() const = 0;

        /**
         * @brief Return SPMod menu manager.
         * 
         * @return              Menu manager.
         */
        virtual IMenuMngr *getMenuManager() const = 0;

        /**
         * @brief Return SPMod player manager.
         *
         * @return              Player manager.
         */
        virtual IPlayerMngr *getPlayerManager() const = 0;

        /**
         * @brief Return SPMod utils funcs.
         *
         * @return              Utils funcs.
         */
        virtual IUtils *getUtils() const = 0;

        /**
         * @brief Returns current SourcePawn environment.
         *
         * @return        SourcePawn environment.
         */
        virtual SourcePawn::ISourcePawnEnvironment *getSPEnvironment() const = 0;

        /**
         * @brief Formats a string according to the SPMod format rules.
         *
         * @param buffer        Destination buffer.
         * @param length        Length of buffer.
         * @param format        Formatting string.
         * @param ctx           Plugin context.
         * @param params        Params list passed by native.
         * @param param         Index of param which contains first formatting argument.
         *
         * @return              Number of characters written.
         */
        virtual unsigned int formatString(char *buffer,
                                          std::size_t length,
                                          const char *format,
                                          SourcePawn::IPluginContext *ctx,
                                          const cell_t *params,
                                          std::size_t param) const = 0;

        /**
         * @brief Registers module's interface.
         *
         * @param interface     Interface's address.
         *
         * @return              True if registered successfully, false otherwise.
         */
        virtual bool registerInterface(IInterface *interface) = 0;

        /**
         * @brief Gets a module's interface.
         *
         * @param name          Name of the interface to look up for.
         *
         * @return              True if registered successfully, false otherwise.
         */
        virtual IInterface *getInterface(const char *name) const = 0;

    protected:
        virtual ~ISPGlobal() {};
    };

    enum class ExtQueryValue : uint8_t
    {
        DontLoad = 0,
        SPModExt = 1,
        MetaExt = 2
    };

    /**
     * @brief Entry point for modules to obtain access to SPMod API.
     *
     * @return
     */
    SPMOD_API ExtQueryValue SPMod_Query(ISPGlobal *spmodInstance);
    using fnSPModQuery = ExtQueryValue (*)(ISPGlobal *spmodInstance);

    /**
     * @brief Called after SPMod_Query().
     *
     * @note Here extension should ask for interfaces.
     *
     * @return True if extension should be loaded, false otherwise.
     */
    SPMOD_API bool SPMod_Init();
    using fnSPModInit = bool (*)();

    /**
     * @brief Called on extension unloading. (mapchange, server shutdown)
     */
    SPMOD_API void SPMod_End();
    using fnSPModEnd = void (*)();
}
