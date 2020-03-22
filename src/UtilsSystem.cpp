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

#include "spmod.hpp"

std::size_t Utils::strCopy(char *buffer, std::size_t size, std::string_view src) const
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

std::string Utils::strReplaced(std::string_view source, std::string_view from, std::string_view to) const
{
    std::string temp(source);
    std::size_t start_pos = temp.find(from);
    if (start_pos == std::string::npos)
        return temp;
    temp.replace(start_pos, from.length(), to);
    return temp;
}

void Utils::ShowMenu(const Edict *pEdict, std::uint32_t slots, std::uint32_t time, std::string_view menu)
{
    static constexpr std::size_t maxStringToSend = 175;
    static constexpr std::size_t maxMenuLength = 512;

    if (!gmsgShowMenu)
        return; // some games don't support ShowMenu (Firearms)

    std::size_t currentPos = 0;
    std::size_t menuLength = (menu.length() > maxMenuLength) ? maxMenuLength : menu.length();
    std::string_view buffer = (menuLength > maxStringToSend) ? menu.substr(0, maxStringToSend) : menu;
    do
    {
        menuLength -= buffer.length();
        MESSAGE_BEGIN(MSG_ONE, gmsgShowMenu, nullptr, pEdict->getInternalEdict());
        WRITE_SHORT(slots);
        WRITE_CHAR(time);
        WRITE_BYTE((menuLength > 0) ? true : false);
        WRITE_STRING(buffer.data());
        MESSAGE_END();

        currentPos += buffer.length();
        buffer = menu.substr(currentPos, (menuLength > maxStringToSend) ? maxStringToSend : std::string_view::npos);
    } while (menuLength > 0);
}
