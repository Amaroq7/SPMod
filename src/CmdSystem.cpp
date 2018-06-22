/*  SPMod - SourcePawn Scripting Engine for Half-Life
 *  Copyright (C) 2018  SPMod Development Team
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

ClientCommand::ClientCommand(size_t id,
                             std::string_view cmd,
                             std::string_view info,
                             SourcePawn::IPluginFunction *func,
                             uint32_t flags) : m_flags(flags)
{
    m_id = id;
    m_cmd = cmd;
    m_info = info;
    m_func = func;
}

ServerCommand::ServerCommand(size_t id,
                             std::string_view cmd,
                             std::string_view info,
                             SourcePawn::IPluginFunction *func)
{
    m_id = id;
    m_cmd = cmd;
    m_info = info;
    m_func = func;
}