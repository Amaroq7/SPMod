/*
*  Copyright (C) 2018-2021 SPMod Development Team
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

#pragma once

#include "IInterface.hpp"

#include <string>
#include <unordered_set>
#include <memory>

namespace SPMod
{
    class IAccessGroup
    {
    public:
        virtual ~IAccessGroup() = default;

        [[nodiscard]] virtual std::string_view getName() const = 0;
        virtual bool addPermission(std::string_view permission) = 0;
        virtual void removePermission(std::string_view permission) = 0;
        virtual bool hasPermission(std::string_view permission) = 0;

        [[nodiscard]] virtual const std::unordered_set<std::string> &getPerms() const = 0;
    };

    class IGroupMngr : public ISPModInterface
    {
    public:
        static constexpr std::uint16_t MAJOR_VERSION = 0;
        static constexpr std::uint16_t MINOR_VERSION = 0;

        static constexpr std::uint32_t VERSION = (MAJOR_VERSION << 16 | MINOR_VERSION);

        /**
         * @brief Gets interface's name.
         *
         * @return        Interface's name.
         */
        [[nodiscard]] std::string_view getName() const override
        {
            return "IGroupMngr";
        }

        /**
         * @brief Gets interface's version.
         *
         * @note The first 16 most significant bits represent major version, the rest represent minor version.
         *
         * @return        Interface's version.
         */
        [[nodiscard]] std::uint32_t getVersion() const override
        {
            return VERSION;
        }

        ~IGroupMngr() override = default;

        virtual std::weak_ptr<IAccessGroup> createGroup(std::string_view name) = 0;
        virtual std::weak_ptr<IAccessGroup> getGroup(std::string_view name) = 0;
        virtual void removeGroup(std::weak_ptr<IAccessGroup> group) = 0;
        virtual std::weak_ptr<IAccessGroup> cloneGroup(std::weak_ptr<IAccessGroup> group, std::string_view newname) = 0;
    };
}