/*
 *  Copyright (C) 2020 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SPMod is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with SPMod.  If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once

namespace SPMod
{
    enum class DirType : std::uint8_t
    {
        Home = 0,
        Dlls,
        Exts,
        Plugins,
        Logs,
        Configs
    };

    enum class ModName : std::uint8_t
    {
        Valve = 0,
        Cstrike,
        CZero,
        DoD,
        TFC
    };

    enum class NavigationType : std::int8_t
    {
        None = 0,
        Back = -1,
        Next = -2,
        Exit = -3
    };

    enum class LogLevel : std::uint8_t
    {
        Debug,
        Info,
        Warning,
        Error
    };

    /**
     * @brief Maximum number of supported players.
     */
    constexpr std::uint32_t MAX_PLAYERS = 32U;

} // namespace SPMod