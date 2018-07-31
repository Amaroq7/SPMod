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

#include <cstdint>

namespace SPMod
{
    class IInterface
    {
    public:
        /**
         * @brief Gets interface's name.
         *
         * @return              Interface's name.
         */
        virtual const char *getInterfaceName() const = 0;

        /**
         * @brief Gets interface's version.
         *
         * @note The first 16 most significant bits represent major version, the rest represent minor version.
         *
         * @return              Interface's version.
         */
        virtual uint32_t getInterfaceVersion() const = 0;

        /**
         * @brief Gets interface's implementation.
         *
         * @return              Interface's implementation, nullptr if not provided.
         */
        virtual void *getInterfaceImpl() const
        {
            return nullptr;
        }

        /**
         * @brief Check if requested version is compatible.
         *
         * @param reqversion    Version to be checked.
         *
         * @return              True if is compatible, false otherwise.
         */
        virtual bool isVersionCompatible(uint32_t reqversion) const
        {
            return (reqversion > getInterfaceVersion() ? false : true);
        }

    protected:
        virtual ~IInterface() {}
    };

    class ISPModInterface SPMOD_FINAL : public IInterface
    {
    public:
        /**
         * @brief Check if requested version is compatible.
         *
         * @param reqversion    Version to be checked.
         *
         * @return              True if is compatible, false otherwise.
         */
        bool isVersionCompatible(uint32_t reqversion) const override
        {
            uint16_t majorReqVer = reqversion >> 16;

            if (majorReqVer != (getInterfaceVersion() >> 16))
                return false;

            uint16_t minorReqVer = reqversion & 0xFFFF;

            if (minorReqVer > (getInterfaceVersion() & 0xFFFF))
                return false;

            return true;
        }

    protected:
        virtual ~ISPModInterface() {}
    };
}