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
    class IUtils SPMOD_FINAL : public ISPModInterface
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
        const char *getName() const override
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
        uint32_t getVersion() const override
        {
            return VERSION;
        }

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
        virtual std::size_t strCopy(char *buffer,
                                    std::size_t size,
                                    const char *src) const = 0;
        
        /**
         * @brief Creates new string with replaced part.
         *
         * @param buffer    Pointer to buffer where string will be stored.
         * @param size      Size of the buffer.
         * @param source    Pointer to string where part will be replaced.
         * @param from      Pointer to string to search for.
         * @param to        Pointer to string to replace the search string from.
         *
         * @return          Written chars to buffer.
         */
        virtual std::size_t strReplaced(char *buffer,
                                        std::size_t size,
                                        const char *source,
                                        const char *from,
                                        const char *to) const = 0;

    protected:
        virtual ~IUtils() = default;
    };
}
