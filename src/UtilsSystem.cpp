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
#include "UtilsSystem.hpp"

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

void Utils::ShowMenu(const Engine::Edict *pEdict, std::uint32_t slots, std::uint32_t time, std::string_view menu)
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
        MESSAGE_BEGIN(MSG_ONE, gmsgShowMenu, nullptr, *pEdict);
        WRITE_SHORT(slots);
        WRITE_CHAR(time);
        WRITE_BYTE((menuLength > 0) ? true : false);
        WRITE_STRING(buffer.data());
        MESSAGE_END();

        currentPos += buffer.length();
        buffer = menu.substr(currentPos, (menuLength > maxStringToSend) ? maxStringToSend : std::string_view::npos);
    } while (menuLength > 0);
}

void Utils::trimMultiByteChar(std::string &str)
{
    std::size_t strLen = str.length();

    // Last character is not multibyte
    if (!(static_cast<std::uint8_t>(str[strLen - 1]) & 0x80U))
    {
        return;
    }

    if (strLen < 2)
    {
        // If it is 4 bytes or 3 bytes character, clear it since string has only 2 bytes, otherwise return unchanged
        if (static_cast<std::uint8_t>(str[0]) & 0xF0U || static_cast<std::uint8_t>(str[0]) & 0xE0U)
        {
            str.clear();
            return;
        }
        return;
    }

    std::size_t pos = strLen - 1;

    while ((static_cast<std::uint8_t>(str[pos]) & 0xC0U) == 0x80U)
    {
        pos--;
    }

    switch (static_cast<std::uint8_t>(str[pos]) & 0xF0U)
    {
        case 0xC0:
        case 0xD0:
        {
            if (strLen - pos < 2)
            {
                str = str.substr(0, pos);
            }
            break;
        }
        case 0xE0:
        {
            if (strLen - pos < 3)
            {
                str = str.substr(0, pos);
            }
            break;
        }
        case 0xF0:
        {
            if (strLen - pos < 4)
            {
                str = str.substr(0, pos);
            }
            break;
        }
    }
}

void Utils::sendTextMsg(std::string_view message, TextMsgDest msgDest, Engine::Edict *edict)
{
    static std::uint8_t msgTextMsgId = GET_USER_MSG_ID(PLID, "TextMsg", nullptr);
    if (!msgTextMsgId)
        return;				// :TODO: Maybe output a warning log?

    constexpr std::size_t maxLength = 187;

    if (edict)
    {
        MESSAGE_BEGIN(MSG_ONE, msgTextMsgId, nullptr, *edict);
    }
    else
    {
        MESSAGE_BEGIN(MSG_BROADCAST, msgTextMsgId);
    }

    WRITE_BYTE(static_cast<std::uint8_t>(msgDest));	// 1 byte
    WRITE_STRING("%s");	// 3 bytes (2 + EOS)
    WRITE_STRING(message.substr(0, maxLength).data());	// max 188 bytes (187 + EOS)
    MESSAGE_END();		// max 192 bytes
}