/*
 *  Copyright (C) 2020-2021 Metamod++ Development Team
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

#include "StdFSWrapper.hpp"
#include <string_view>

namespace Metamod 
{
    class IPlugin 
    {
        public:
            virtual ~IPlugin() = default;
            virtual std::uint32_t getInterfaceVersion() const = 0;
            virtual std::string_view getName() const = 0;
            virtual std::string_view getVersion() const = 0;
            virtual std::string_view getDate() const = 0;
            virtual std::string_view getAuthor() const = 0;
            virtual std::string_view getUrl() const = 0;
            virtual std::string_view getLogTag() const = 0;

            [[nodiscard]] const fs::path &getPath() const
            {
                return m_path;
            }

#if defined META_CORE
            void setPath(fs::path &&pluginPath)
            {
                m_path = std::move(pluginPath);
            }
#endif

        protected:
            fs::path m_path;
    };
}