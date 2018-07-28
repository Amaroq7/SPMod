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

#include "spmod.hpp"

std::size_t Utils::strCopyCore(char *buffer,
                               std::size_t size,
                               std::string_view src) const
{
#if defined __STDC_LIB_EXT1__ || defined SP_MSVC
    #if defined SP_MSVC
    strncpy_s(buffer, size, src.data(), _TRUNCATE);
    #else
    strncpy_s(buffer, size, src.data(), size - 1);
    #endif // SP_MSVC
#else
    std::strncpy(buffer, src.data(), size);
    buffer[size - 1] = '\0';
#endif // __STDC_LIB_EXT1__ || defined SP_MSVC

    std::size_t writtenChars = size - 1;
    if (src.length() < writtenChars)
        writtenChars = src.length();

    return writtenChars;
}

std::size_t Utils::strCopy(char *buffer,
                           std::size_t size,
                           const char *src) const
{
    return strCopyCore(buffer, size, src);
}
