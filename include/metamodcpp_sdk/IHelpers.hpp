/*
 *  Copyright (C) 2020 Metamod++ Development Team
 *
 *  This file is part of Metamod++.
 *
 *  Metamod++ is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  Metamod++ is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with Metamod++.  If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once

#if !defined META_DONT_INCLUDE_ENUM_HELPERS

#include <type_traits>

template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
inline constexpr T operator&(const T lhs, const T rhs)
{
  using enumType = std::underlying_type_t<T>;
  return static_cast<T>(static_cast<enumType>(lhs) & static_cast<enumType>(rhs));
}

template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
inline constexpr T operator|(const T lhs, const T rhs)
{
  return static_cast<T>(static_cast<std::underlying_type_t<T>>(lhs) | static_cast<std::underlying_type_t<T>>(rhs));
}

template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
inline constexpr T operator^(const T lhs, const T rhs)
{
  return static_cast<T>(static_cast<std::underlying_type_t<T>>(lhs) ^ static_cast<std::underlying_type_t<T>>(rhs));
}

#endif
