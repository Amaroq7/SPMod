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

namespace SPMod
{
    class IPluginMngr;

    class IInterface
    {
    public:
        /**
         * @brief Gets interface's name.
         *
         * @return              Interface's name.
         */
        virtual const char *getName() const = 0;

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
        virtual const char *getAuthor() const = 0;

        /**
         * @brief Gets interface's url.
         *
         * @return              Interface's url.
         */
        virtual const char *getUrl() const = 0;

        /**
         * @brief Gets name of the extension that implements the interface.
         *
         * @return              Extension's name.
         */
        virtual const char *getExtName() const = 0;

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

    protected:
        virtual ~IInterface() = default;
    };

    class ISPModInterface SPMOD_FINAL : public IInterface
    {
    public:
        const char *getAuthor() const override final
        {
            return "SPMod Development Team";
        }

        const char *getUrl() const override final
        {
            return "https://github.com/Amaroq7/SPMod";
        }

        const char *getExtName() const override final
        {
            return "SPMod";
        }

        bool isVersionCompatible(std::uint32_t reqversion) const override final
        {
            std::uint16_t majorReqVer = reqversion >> 16;

            if (majorReqVer != (getVersion() >> 16))
                return false;

            std::uint16_t minorReqVer = reqversion & 0xFFFF;

            if (minorReqVer > (getVersion() & 0xFFFF))
                return false;

            return true;
        }

        IPluginMngr *getPluginMngr() const final
        {
            return nullptr;
        }

    protected:
        virtual ~ISPModInterface() = default;
    };
}
