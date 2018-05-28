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
    class IModuleInterface;
    class INative
    {
    public:
        /**
         * @brief Returns name of the native.
         *
         * @return        Native name.
         */
        virtual const char *getName() const = 0;

        /**
         * @brief Returns identity of the owner.
         *
         * @return        Owner identity.
         */
        virtual const char *getOwner() const = 0;

        /**
         * @brief Returns plugin function to be executed when native is called.
         *
         * @note          If nullptr then native was added by module.
         *
         * @return        Function pointer.
         */
        virtual SourcePawn::IPluginFunction *getFunc() const = 0;

    protected:
        virtual ~INative() {};
    };

    class INativeMngr
    {
    public:
        /**
         * @brief Registers list of natives.
         *
         * @note               If native in list is already registered further natives will not be registered
         *                     and function returns false.
         *
         * @param interface    Pointer to module interface.
         * @param nativeslist  Natives list to add.
         *
         * @return             True if all natives has been registered successfully, false otherwise.
         */
        virtual bool addNatives(IModuleInterface *interface, const sp_nativeinfo_t *nativeslist) = 0;

        /**
         * @brief Returns native.
         *
         * @param name    Native name
         *
         * @return        Native pointer if found, nullptr if not found.
         */
        virtual INative *getNative(const char *name) const = 0;

    protected:
        virtual ~INativeMngr() {};
    };
}