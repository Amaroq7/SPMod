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

#include "spmod.hpp"

std::size_t Utils::strCopyCore(char *buffer, std::size_t size, std::string_view src) const
{
#if defined __STDC_LIB_EXT1__ || defined SP_MSVC
    #if defined SP_MSVC
    strncpy_s(buffer, size, src.data(), _TRUNCATE);
    #else
    strncpy_s(buffer, size, src.data(), size - 1);
    #endif // SP_MSVC
#else
    std::strncpy(buffer, src.data(), size);
#endif // __STDC_LIB_EXT1__ || defined SP_MSVC

    std::size_t writtenChars = size - 1;
    if (src.length() < writtenChars)
        writtenChars = src.length();

#if !defined SP_MSVC
    buffer[writtenChars] = '\0';
#endif
    return writtenChars;
}

std::string Utils::strReplacedCore(std::string_view source, std::string_view from, std::string_view to) const
{
    std::string temp(source);
    std::size_t start_pos = temp.find(from);
    if (start_pos == std::string::npos)
        return temp;
    temp.replace(start_pos, from.length(), to);
    return temp;
}

std::size_t Utils::strCopy(char *buffer, std::size_t size, const char *src) const
{
    return strCopyCore(buffer, size, src);
}

std::size_t
    Utils::strReplaced(char *buffer, std::size_t size, const char *source, const char *from, const char *to) const
{
    return strCopyCore(buffer, size, strReplacedCore(source, from, to));
}

void Utils::ShowMenu(std::shared_ptr<Edict> pEdict, int slots, int time, const char *menu, std::size_t menuLength)
{
    static constexpr std::size_t bufferSize = 175;

    if (!gmsgShowMenu)
        return; // some games don't support ShowMenu (Firearms)

    std::size_t currentPos = 0;
    static char bufferOutput[bufferSize] = {};

    do
    {
        std::size_t writtenChars = strCopyCore(bufferOutput, bufferSize, std::string_view(menu, currentPos));
        currentPos += writtenChars;
        menuLength -= writtenChars;

        MESSAGE_BEGIN(MSG_ONE, gmsgShowMenu, nullptr, pEdict->getInternalEdict());
        WRITE_SHORT(slots);
        WRITE_CHAR(time);
        WRITE_BYTE((menuLength > 0) ? true : false);
        WRITE_STRING(bufferOutput);
        MESSAGE_END();
    } while (menuLength > 0);
}
