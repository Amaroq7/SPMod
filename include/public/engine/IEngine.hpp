/*
 *  Copyright (C) 2020 SPMod Development Team
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

namespace SPMod::Engine
{
    class IEngine : public ISPModInterface
    {
    public:
        static constexpr std::uint16_t MAJOR_VERSION = 0;
        static constexpr std::uint16_t MINOR_VERSION = 0;

        static constexpr std::uint32_t VERSION = (MAJOR_VERSION << 16 | MINOR_VERSION);

        /**
         * @brief Gets interface's name.
         *
         * @return        Interface's name.
         */
        std::string_view getName() const override
        {
            return "IEngine";
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

        virtual ~IEngine() = default;

        /**
         * @brief Gets edict.
         *
         * @param index         Edict's index.
         *
         * @return              Edict.
         */
        virtual IEdict *getEdict(std::uint32_t index) = 0;

        /**
         * @brief Returns engine globals.
         *
         * @return              Engine globals.
         */
        virtual IGlobals *getGlobals() const = 0;

        /**
         * @brief Returns engine funcs.
         *
         * @param hook          True if functions should call any hooks.
         *
         * @return              Engine functions.
         */
        virtual IFuncs *getFuncs(bool hook = false) const = 0;

        /**
         * @brief Creates trace result.
         *
         * @note                Must be freed using freeTraceResult().
         *
         * @return              Trace result.
         */
        virtual ITraceResult *createTraceResult() = 0;

        /**
         * @brief Frees trace result.
         *
         * @param tr              Trace result to be freed.
         *
         */
        virtual void freeTraceResult(ITraceResult *tr) = 0;
    };
} // namespace SPMod::Engine
