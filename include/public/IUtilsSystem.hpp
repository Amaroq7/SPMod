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
    class IUtils : public ISPModInterface
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
        [[nodiscard]] std::string_view getName() const override
        {
            return "IUtils";
        }

        /**
         * @brief Gets interface's version.
         *
         * @note The first 16 most significant bits represent major version, the rest represent minor version.
         *
         * @return        Interface's version.
         */
        [[nodiscard]] std::uint32_t getVersion() const override
        {
            return VERSION;
        }

        ~IUtils() override = default;

        /**
         * @brief Copies string to buffer.
         *
         * @note Truncates string whenever it exceeds buffer size.
         *
         * @param buffer    Pointer to buffer where string will be stored.
         * @param size      Size of the buffer.
         * @param src       String to be copied.
         *
         * @return          Written chars to buffer.
         */
        virtual std::size_t strCopy(char *buffer, std::size_t size, std::string_view src) const = 0;

        /**
         * @brief Creates new string with replaced part.
         *
         * @param source    String where part will be replaced.
         * @param from      String to search for.
         * @param to        String to replace the search string from.
         *
         * @return          Modified string.
         */
        [[nodiscard]] virtual std::string strReplaced(std::string_view source, std::string_view from, std::string_view to) const = 0;
    };
} // namespace SPMod
