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

#include "StandardHeaders.hpp"
#include "IInterface.hpp"

#include <anubis/observer_ptr.hpp>

namespace Anubis
{
    class IAnubis;
}

namespace SPMod
{
    class IPluginMngr;
    class ISPGlobal;

    class IModuleInterface
    {
    public:
        virtual ~IModuleInterface() = default;

        /**
         * @brief Gets interface's name.
         *
         * @return              Interface's name.
         */
        [[nodiscard]] virtual std::string_view getName() const = 0;

        /**
         * @brief Gets interface's version.
         *
         * @return              Interface's version.
         */
        [[nodiscard]] virtual std::uint32_t getVersion() const = 0;

        /**
         * @brief Gets interface's author.
         *
         * @return              Interface's author.
         */
        [[nodiscard]] virtual std::string_view getAuthor() const = 0;

        /**
         * @brief Gets interface's url.
         *
         * @return              Interface's url.
         */
        [[nodiscard]] virtual std::string_view getUrl() const = 0;

        /**
         * @brief Gets name of the extension that implements the interface.
         *
         * @return              Extension's name.
         */
        [[nodiscard]] virtual std::string_view getExtName() const = 0;

        /**
         * @brief Check if requested version is compatible.
         *
         * @param reqversion    Version to be checked.
         *
         * @return              True if is compatible, false otherwise.
         */
        [[nodiscard]] virtual bool isVersionCompatible(std::uint32_t reqversion) const
        {
            return reqversion <= getVersion();
        }

        /**
         * @brief Gets interface's implementation.
         *
         * @return              Interface's implementation.
         */
        [[nodiscard]] virtual std::any getImplementation() const = 0;
    };

    class IAdapterInterface
    {
    public:
        virtual ~IAdapterInterface() = default;

        /**
         * @brief Gets interface's name.
         *
         * @return              Interface's name.
         */
        [[nodiscard]] virtual std::string_view getName() const = 0;

        /**
         * @brief Gets interface's version.
         *
         * @return              Interface's version.
         */
        [[nodiscard]] virtual std::uint32_t getVersion() const = 0;

        /**
         * @brief Gets interface's author.
         *
         * @return              Interface's author.
         */
        [[nodiscard]] virtual std::string_view getAuthor() const = 0;

        /**
         * @brief Gets interface's url.
         *
         * @return              Interface's url.
         */
        [[nodiscard]] virtual std::string_view getUrl() const = 0;

        /**
         * @brief Gets name of the extension that implements the interface.
         *
         * @return              Extension's name.
         */
        [[nodiscard]] virtual std::string_view getExtName() const = 0;

        /**
         * @brief Check if requested version is compatible.
         *
         * @param reqversion    Version to be checked.
         *
         * @return              True if is compatible, false otherwise.
         */
        [[nodiscard]] virtual bool isVersionCompatible(std::uint32_t reqversion) const
        {
            return reqversion <= getVersion();
        }

        /**
         * @brief Gets plugin manager.
         *
         * @return              Plugin manager.
         */
        [[nodiscard]] virtual nstd::observer_ptr<IPluginMngr> getPluginMngr() const = 0;
    };

    class ISPModInterface
    {
    public:
        virtual ~ISPModInterface() = default;

        [[nodiscard]] virtual std::string_view getName() const = 0;
        [[nodiscard]] virtual std::uint32_t getVersion() const = 0;

        [[nodiscard]] virtual std::string_view getAuthor() const final
        {
            return "SPMod Development Team";
        }

        [[nodiscard]] virtual std::string_view getUrl() const final
        {
            return "https://github.com/Amaroq7/SPMod";
        }

        [[nodiscard]] virtual std::string_view getExtName() const final
        {
            return "SPMod";
        }

        [[nodiscard]] virtual bool isVersionCompatible(std::uint32_t reqversion) const final
        {
            if (std::uint16_t majorReqVer = reqversion >> 16; majorReqVer != (getVersion() >> 16))
                return false;

            if (std::uint16_t minorReqVer = reqversion & 0xFFFF; minorReqVer > (getVersion() & 0xFFFF))
                return false;

            return true;
        }
    };

    enum class ExtQueryValue : std::uint8_t
    {
        DontLoad = 0,
        SPModModule,
        SPModAdapter
    };

    /**
     * @brief Entry point for modules to obtain access to SPMod API.
     *
     * @return
     */
    SPMOD_API ExtQueryValue Query(nstd::observer_ptr<ISPGlobal> spmodInstance, Anubis::IAnubis *anubisAPI);
    using fnSPModQuery = ExtQueryValue (*)(ISPGlobal *spmodInstance, Metamod::IMetamod *metaAPI);

    /**
     * @brief Called after SPMod_Query().
     *
     * @note Here extension should ask for interfaces.
     *
     * @return True if extension should be loaded, false otherwise.
     */
    SPMOD_API bool Init();
    using fnSPModInit = bool (*)();

    /**
     * @brief Called on extension unloading. (server shutdown)
     */
    SPMOD_API void Shutdown();
    using fnSPModEnd = void (*)();
} // namespace SPMod
