/*
 *  Copyright (C) 2018-2020 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SPMod is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with SPMod.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

namespace SPMod
{
    class ICvar
    {
    public:
        using Callback =
            std::function<void(const ICvar *const cvar, std::string_view old_value, std::string_view new_value)>;
        /**
         * Cvar flags (from engine)
         */
        enum class Flags
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

            // It's a server cvar, but we don't send the data since it's a password, etc.  Sends 1 if it's not
            // bland/zero, 0 otherwise as value
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

        virtual ~ICvar() = default;

        /**
         * @brief Returns name of the cvar.
         *
         * @return        Cvar name.
         */
        virtual std::string_view getName() const = 0;

        /**
         * @brief Returns flags of the cvar
         *
         * @return        Flags of the cvar.
         */
        virtual Flags getFlags() const = 0;

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
        virtual void setValue(std::int32_t val) = 0;

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
        virtual void addCallback(Callback callback) = 0;

        /**
         * @brief Return cvar value as integer.
         *
         * @return    Integer cvar value
         */
        virtual std::int32_t asInt() const = 0;

        /**
         * @brief Return cvar value  as Float.
         *
         * @return    Float cvar value
         */
        virtual float asFloat() const = 0;

        /**
         * @brief Return cvar value as String.
         *
         * @return    String cvar value
         */
        virtual std::string_view asString() const = 0;
    };

    class ICvarMngr : public ISPModInterface
    {
    public:
        static constexpr uint16_t MAJOR_VERSION = 0;
        static constexpr uint16_t MINOR_VERSION = 0;

        static constexpr uint32_t VERSION = (MAJOR_VERSION << 16 | MINOR_VERSION);
        /**
         * @brief Gets interface's name.
         *
         * @return        Interface's name.
         */
        std::string_view getName() const override
        {
            return "ICvarMngr";
        }

        /**
         * @brief Gets interface's version.
         *
         * @note The first 16 most significant bits represent major version, the rest represent minor version.
         *
         * @return        Interface's version.
         */
        std::uint32_t getVersion() const override
        {
            return VERSION;
        }

        virtual ~ICvarMngr() = default;

        /*
         * @brief Registers cvar.
         *
         * @param name           Name of the cvar.
         * @param value_type     Type of the value
         * @param flags          Engine flags for cvar
         *
         * @return               Cvar pointer, nullptr if failed.
         */
        virtual ICvar *registerCvar(std::string_view name, std::string_view value, ICvar::Flags flags) = 0;

        /*
         * @brief Find cvar.
         *
         * @param name           Name of the cvar.
         *
         * @return               Cvar pointer, nullptr if failed.
         */
        virtual ICvar *findCvar(std::string_view name) = 0;
    };
} // namespace SPMod
