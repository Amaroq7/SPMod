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

#include <ISPGlobal.hpp>

namespace Metamod::Engine
{
    class IEdict;
}

using namespace SPMod;

class Utils final : public IUtils
{
public:
    // IUtils
    std::size_t strCopy(char *buffer, std::size_t size, std::string_view src) const override;
    std::string strReplaced(std::string_view source, std::string_view from, std::string_view to) const override;

    static void ShowMenu(Metamod::Engine::IEdict *pEdict, std::int16_t slots, std::int8_t time, std::string_view menu);
    static void trimMultiByteChar(std::string &str);
    static void sendTextMsg(std::string_view message, TextMsgDest msgDest, Metamod::Engine::IEdict *edict = nullptr);
};
