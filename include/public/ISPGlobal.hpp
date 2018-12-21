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

#pragma once

#include <cinttypes>
#include <IHelpers.hpp>
#include <IInterface.hpp>
#include <IForwardSystem.hpp>
#include <ICvarSystem.hpp>
#include <ITimerSystem.hpp>
#include <IMenuSystem.hpp>
#include <IUtilsSystem.hpp>
#include <IPlayerSystem.hpp>
#include <ILoggerSystem.hpp>
#include <ICmdSystem.hpp>
#include <IPluginSystem.hpp>
#include <INativeProxy.hpp>

namespace SPMod
{
    enum class DirType : std::uint8_t
    {
        Home = 0,
        Dlls,
        Exts,
        Plugins,
        Logs
    };

    class ISPGlobal SPMOD_FINAL : public ISPModInterface
    {
    public:
        static constexpr std::uint16_t MAJOR_VERSION = 0;
        static constexpr std::uint16_t MINOR_VERSION = 0;

        static constexpr std::uint32_t VERSION = (MAJOR_VERSION << 16 | MINOR_VERSION);

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
        std::uint32_t getVersion() const override
        {
            return VERSION;
        }

        /**
         * @brief Returns home dir of SPMod.
         *
         * @return        Home dir.
         */
        virtual const char *getPath(DirType type) const = 0;

        /**
         * @brief Returns name of the mod.
         *
         * @return        Mod name.
         */
        virtual const char *getModName() const = 0;

        /**
         * @brief Checks if plugins can precache resources.
         *
         * @return        True if they are allowed to, false otherwise.
         */
        virtual bool canPluginsPrecache() const = 0;

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
         * @brief Returns SPMod timer manager.
         *
         * @return              Timer manager.
         */
        virtual ITimerMngr *getTimerManager() const = 0;

        /**
         * @brief Returns SPMod menu manager.
         * 
         * @return              Menu manager.
         */
        virtual IMenuMngr *getMenuManager() const = 0;

        /**
         * @brief Returns SPMod logger manager.
         *
         * @return              Logger manager.
         */
        virtual ILoggerMngr *getLoggerManager() const = 0;

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
         * @brief Return native proxy.
         *
         * @return              Native proxy.
         */
        virtual INativeProxy *getNativeProxy() const = 0;

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
        virtual ~ISPGlobal() = default;
    };

    enum class ExtQueryValue : uint8_t
    {
        DontLoad = 0,
        SPModExt = 1
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
