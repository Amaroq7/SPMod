/*
 *  Copyright (C) 2018-2020 SPMod Development Team

 *  This file is part of SPMod.

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

std::uint32_t MetaFuncs::getUsrMsgId(std::string_view msgName) const
{
    return GET_USER_MSG_ID(PLID, msgName.data(), nullptr);
}

std::string_view MetaFuncs::getUsrMsgName(std::uint32_t msgId) const
{
    return GET_USER_MSG_NAME(PLID, msgId, nullptr);
}
