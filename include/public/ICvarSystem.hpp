/*
*  Copyright (C) 2018 SPMod Development Team
*
*  This file is part of SPMod.
*
*  SPMod is free software: you can redistribute it and/or modify
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

#include <type_traits>

namespace SPMod {
    class ICvar SPMOD_FINAL
    {        
    public:     
        typedef void cvarCallback_t(const ICvar *cvar, std::string_view old_value, std::string_view new_value);
        /**
         * Cvar flags (from engine)
         */
        enum Flags
        {
            // no extra flags
                    None = 0,

            // set to cause it to be saved to vars.rc
                    Archive = (1 << 0),

            // changes the client's info string
                    UserInfo = (1 << 1),

            // notifies players when changed
                    Server = (1 << 2),

            // defined by external DLL
                    ExtDll = (1 << 3),

            // defined by the client dll
                    ClientDll = (1 << 4),

            // It's a server cvar, but we don't send the data since it's a password, etc.  Sends 1 if it's not bland/zero, 0 otherwise as value
                    Protected = (1 << 5),

            // This cvar cannot be changed by clients connected to a multiplayer server.
                    SPOnly = (1 << 6),

            // This cvar's string cannot contain unprintable characters ( e.g., used for player name etc ).
                    PrintableOnly = (1 << 7),

            // If this is a FCVAR_SERVER, don't log changes to the log file / console if we are creating a log
                    Unlogged = (1 << 8),

            // strip trailing/leading white space from this cvar
                    NoExtraWhiteSpace = (1 << 9)
        };

        /**
        * @brief Returns name of the cvar.
        *
        * @return        Cvar name.
        */
        virtual const char* getName() const = 0;

        /**
        * @brief Returns flags of the cvar
        *
        * @return        flags of the cvar.
        */
        virtual Flags getFlags() const = 0;
        /**
        * @brief Returns id of plugin.
        *
        * @return        Plugin id.
        */
        virtual size_t getId() const = 0;
        /**
        * @brief Set's cvar value by string
        *
        * @param val           New value of the cvar.
        *
        * @noreturn
        */
        virtual void setValue(std::string_view val) = 0;
        /**
        * @brief Set's cvar value by int
        *
        * @param val           New value of the cvar.
        *
        * @noreturn
        */
        virtual void setValue(int val) = 0;
        /**
        * @brief Set's cvar value by float
        *
        * @param val           New value of the cvar.
        *
        * @noreturn
        */
        virtual void setValue(float val) = 0;
        /**
        * @brief Set's cvar flags
        *
        * @param flags           New flags of the cvar.
        *
        * @noreturn
        */
        virtual void setFlags(Flags flags) = 0;
        /**
        * @brief Add callback for cvar (for modules)
        *
        * @noreturn
        */        
        virtual void addCallback(cvarCallback_t *callback) = 0;
        /**
        * @brief Add callback for cvar (for modules)
        *
        * @noreturn
        */
        virtual void addPluginCallback(SourcePawn::IPluginFunction *callback) = 0;
        /**
        * @brief Return cvar value as integer.
        *
        * @return    Integer cvar value
        */
        virtual int            asInt() const = 0;
        /**
        * @brief Return cvar value  as Float.
        *
        * @return    Float cvar value
        */
        virtual float        asFloat() const = 0;
        /**
        * @brief Return cvar value as String.
        *
        * @return    String cvar value
        */
        virtual std::string asString() const = 0;
    protected:
        virtual ~ICvar() {}
    };

    struct CvarList
    {
        /* Current cvar */
        ICvar *cvar;
        /* Next cvar in the list, nullptr if there is no more cvars */
        CvarList *next;
    };

    class ICvarMngr SPMOD_FINAL
    {
    public:
        /*
        * @brief Registers cvar or return existed cached cvar.
        *
        * @param name           Name of the cvar.
        * @param type           Cvar type
        * @param value_type     Type of the value
        * @param flags          Engine flags for cvar
        *
        * @return          Cvar pointer, nullptr if failed.
        */
        virtual ICvar *registerOrFindCvar(const char *name, char* value, ICvar::Flags flags, bool force_register) = 0;
        /*
        * @brief Find cvar.
        *
        * @param name           Name of the cvar.
        *
        * @return          Cvar pointer, nullptr if failed.
        */
        virtual ICvar *findCvar(const char *name ) = 0;
        /*
        * @brief Find cvar.
        *
        * @param name           Id of the cvar.
        *
        * @return          Cvar pointer, nullptr if failed.
        */
        virtual ICvar *findCvar(size_t id) = 0;
        /*
        * @brief Sets cvar callback
        *
        * @param cvar           Pointer to cvar
        * @param callback           callback function
        *
        * @return          CvarList , nullptr if failed.
        */
        virtual bool setCvarCallback(ICvar *cvar, ICvar::cvarCallback_t *callback) = 0;

        /*
        * @brief Sets cvar callback
        *
        * @param cvar               Pointer to cvar
        * @param callback           callback plugin function
        *
        * @return          CvarList , nullptr if failed.
        */
        virtual bool setCvarCallback(ICvar *cvar, SourcePawn::IPluginFunction *callback) = 0;
    };
};
