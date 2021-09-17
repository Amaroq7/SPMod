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

#include <IAccessSystem.hpp>

#include <string>
#include <unordered_set>
#include <unordered_map>

using namespace SPMod;

class AccessGroup final : public IAccessGroup
{
public:
    explicit AccessGroup(std::string_view name);
    ~AccessGroup() final = default;
    AccessGroup(const AccessGroup &other) = delete;
    AccessGroup &operator=(const AccessGroup &other);

    std::string_view getName() const final;

    bool addPermission(std::string_view permission) final;
    void removePermission(std::string_view permission) final;
    bool hasPermission(std::string_view permission) final;

    const std::unordered_set<std::string> &getPerms() const final;

private:
    std::string m_name;
    std::unordered_set<std::string> m_permissions;
};

class GroupMngr final : public IGroupMngr
{
public:
    ~GroupMngr() final = default;
    std::weak_ptr<IAccessGroup> createGroup(std::string_view name) final;
    std::weak_ptr<IAccessGroup> getGroup(std::string_view name) final;
    void removeGroup(std::weak_ptr<IAccessGroup> group) final;
    std::weak_ptr<IAccessGroup> cloneGroup(std::weak_ptr<IAccessGroup> group, std::string_view newname) final;

private:
    std::unordered_map<std::string, std::shared_ptr<AccessGroup>> m_groups;
};