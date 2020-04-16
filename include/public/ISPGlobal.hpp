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

#pragma once

#include <cinttypes>
#include <cstddef>
#include <functional>
#include <any>
#include <string>
#include <variant>
#include <array>
#include <type_traits>
#include <regex>
#include <charconv>

#include <IHelpers.hpp>

#if __has_include(<filesystem>)
    #include <filesystem>
    // As of GCC 8.1, Clang 7 and MSVC 2019 filesystem is no longer part of experimental
    #if (defined SP_GCC && __GNUC__ >= 8) || (defined SP_CLANG && __clang_major__ >= 7) ||                             \
        (defined SP_MSVC && _MSC_VER >= 1920)
namespace fs = std::filesystem;
    #else // Some compilers still have filesystem within experimental namespace like MSVC 2017
namespace fs = std::experimental::filesystem;
    #endif
#elif __has_include(<experimental/filesystem>)
    #include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
    #error Filesystem header missing
#endif

#include <IInterface.hpp>
#include <IForwardSystem.hpp>
#include <ICvarSystem.hpp>
#include <ITimerSystem.hpp>
#include <IMenuSystem.hpp>
#include <IMessageSystem.hpp>
#include <IEdict.hpp>
#include <IUtilsSystem.hpp>
#include <IPlayerSystem.hpp>
#include <ILoggerSystem.hpp>
#include <ICmdSystem.hpp>
#include <IPluginSystem.hpp>
#include <INativeProxy.hpp>
#include <IEngineFuncs.hpp>
#include <IMetaFuncs.hpp>

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

    class ISPGlobal : public ISPModInterface
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
        std::string_view getName() const override
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
         * @return              Home dir.
         */
        virtual const fs::path &getPath(DirType type) const = 0;

        /**
         * @brief Returns name of the mod.
         *
         * @return              Mod name.
         */
        virtual std::string_view getModName() const = 0;

        /**
         * @brief Checks if plugins can precache resources.
         *
         * @return              True if they are allowed to, false otherwise.
         */
        virtual bool canPluginsPrecache() const = 0;

        /**
         * @brief Gets edict.
         *
         * @param index         Edict's index.
         *
         * @return              Edict.
         */
        virtual IEdict *getEdict(std::uint32_t index) = 0;

        /**
         * @brief Finds plugin.
         *
         * @param pluginname    Plugin name to look up.
         *
         * @return              Plugin pointer or nullptr if not found.
         */
        virtual IPlugin *getPlugin(std::string_view pluginname) const = 0;

        /**
         * @brief Returns SPMod forward manager.
         *
         * @return              Forward manager.
         */
        virtual IForwardMngr *getForwardManager() const = 0;

        /**
         * @brief Returns SPMod cvar manager.
         *
         * @return               Cvar manager.
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
         * @brief Returns SPMod message manager.
         *
         * @return              Message manager.
         */
        virtual IMessageMngr *getMessageManager() const = 0;

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
         * @brief Returns SPMod command manager.
         *
         * @return              Command manager.
         */
        virtual ICommandMngr *getCommandManager() const = 0;

        /**
         * @brief Returns engine funcs.
         *
         * @return              Engine functions.
         */
        virtual IEngineFuncs *getEngineFuncs() const = 0;

        /**
         * @brief Returns engine hooked funcs.
         *
         * @return              Engine hooked functions.
         */
        virtual IEngineFuncsHooked *getEngineHookedFuncs() const = 0;

        /**
         * @brief Returns engine globals.
         *
         * @return              Engine globals.
         */
        virtual IEngineGlobals *getEngineGlobals() const = 0;

        /**
         * @brief Returns engine globals.
         *
         * @return              Engine globals.
         */
        virtual IMetaFuncs *getMetaFuncs() const = 0;

        /**
         * @brief Registers module's interface.
         *
         * @param interface     Interface's implementation.
         *
         * @return              True if registered successfully, false otherwise.
         */
        virtual bool registerModule(IModuleInterface *interface) = 0;

        /**
         * @brief Registers adapter's interface.
         *
         * @param interface     Adapter's implementation.
         *
         * @return              True if registered successfully, false otherwise.
         */
        virtual bool registerAdapter(IAdapterInterface *interface) = 0;

        /**
         * @brief Gets a module's interface.
         *
         * @param name          Name of the interface to look up for.
         *
         * @return              Interface's implementation.
         */
        virtual IModuleInterface *getInterface(std::string_view name) const = 0;

        virtual ~ISPGlobal() = default;
    };
} // namespace SPMod
