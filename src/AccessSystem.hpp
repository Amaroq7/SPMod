/*
 *  Copyright (C) 2018 SPMod Development Team
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

#include "spmod.hpp"
#include <set>

class GroupMngr;

class AccessGroup
{
public:
    AccessGroup(std::string_view name);
    ~AccessGroup() {}

    std::string_view getNameCore() const;

    void addPermisson(std::shared_ptr<std::string> permission);

    void removePermission(std::shared_ptr<std::string> permission);

    const auto &getPerms() const
    {
        return m_permissions;
    }

private:
    std::string m_name;
    std::set<std::shared_ptr<std::string>> m_permissions;
};

class PlayerRole
{
public:
    PlayerRole() {}
    ~PlayerRole() {}

    void attachGroup(std::shared_ptr<AccessGroup> group);
    void removeGroup(std::shared_ptr<AccessGroup> group);
    void attachPermission(std::shared_ptr<std::string> permission);
    void removePermission(std::shared_ptr<std::string> permission);

    bool hasAccess(std::string_view permission);

private:
    std::vector<std::shared_ptr<AccessGroup>> m_groups;

    // single permissions
    std::set<std::shared_ptr<std::string>> m_permissions;
};

class GroupMngr
{
public:
    GroupMngr() {}
    ~GroupMngr() {}

    std::size_t createGroup(std::string_view name);

    std::shared_ptr<std::string> createPermission(std::string_view name);

    std::shared_ptr<std::string> findPermission(std::string_view name);

    std::shared_ptr<AccessGroup> getGroup(std::size_t index);
    int findGroup(std::string_view name);

    void clear();

private:
    std::vector<std::shared_ptr<AccessGroup>> m_groups;
    std::vector<std::shared_ptr<std::string>> m_permissions;
};
