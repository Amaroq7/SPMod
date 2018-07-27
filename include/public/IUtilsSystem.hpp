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
    class IUtils SPMOD_FINAL
    {
    public:
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

    protected:
        virtual ~IUtils() {};
    };
}
