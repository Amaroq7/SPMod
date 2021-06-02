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
        virtual std::string_view getName() const = 0;

        /**
         * @brief Gets interface's version.
         *
         * @return              Interface's version.
         */
        virtual std::uint32_t getVersion() const = 0;

        /**
         * @brief Gets interface's author.
         *
         * @return              Interface's author.
         */
        virtual std::string_view getAuthor() const = 0;

        /**
         * @brief Gets interface's url.
         *
         * @return              Interface's url.
         */
        virtual std::string_view getUrl() const = 0;

        /**
         * @brief Gets name of the extension that implements the interface.
         *
         * @return              Extension's name.
         */
        virtual std::string_view getExtName() const = 0;

        /**
         * @brief Check if requested version is compatible.
         *
         * @param reqversion    Version to be checked.
         *
         * @return              True if is compatible, false otherwise.
         */
        virtual bool isVersionCompatible(std::uint32_t reqversion) const
        {
            return (reqversion > getVersion() ? false : true);
        }

        /**
         * @brief Gets interface's implementation.
         *
         * @return              Interface's implementation.
         */
        virtual void *getImplementation() const = 0;
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
        virtual std::string_view getName() const = 0;

        /**
         * @brief Gets interface's version.
         *
         * @return              Interface's version.
         */
        virtual std::uint32_t getVersion() const = 0;

        /**
         * @brief Gets interface's author.
         *
         * @return              Interface's author.
         */
        virtual std::string_view getAuthor() const = 0;

        /**
         * @brief Gets interface's url.
         *
         * @return              Interface's url.
         */
        virtual std::string_view getUrl() const = 0;

        /**
         * @brief Gets name of the extension that implements the interface.
         *
         * @return              Extension's name.
         */
        virtual std::string_view getExtName() const = 0;

        /**
         * @brief Check if requested version is compatible.
         *
         * @param reqversion    Version to be checked.
         *
         * @return              True if is compatible, false otherwise.
         */
        virtual bool isVersionCompatible(std::uint32_t reqversion) const
        {
            return (reqversion > getVersion() ? false : true);
        }

        /**
         * @brief Gets plugin manager.
         *
         * @return              Plugin manager.
         */
        virtual IPluginMngr *getPluginMngr() const = 0;
    };

    class ISPModInterface
    {
    public:
        virtual ~ISPModInterface() = default;

        virtual std::string_view getName() const = 0;
        virtual std::uint32_t getVersion() const = 0;

        virtual std::string_view getAuthor() const final
        {
            return "SPMod Development Team";
        }

        virtual std::string_view getUrl() const final
        {
            return "https://github.com/Amaroq7/SPMod";
        }

        virtual std::string_view getExtName() const final
        {
            return "SPMod";
        }

        virtual bool isVersionCompatible(std::uint32_t reqversion) const final
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
} // namespace SPMod
