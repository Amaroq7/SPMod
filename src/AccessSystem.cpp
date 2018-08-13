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

#include "spmod.hpp"

AccessGroup::AccessGroup(std::string_view name) : m_name(name)
{}

std::string_view AccessGroup::getNameCore() const
{
    return m_name;
}

void AccessGroup::addPermisson(std::shared_ptr<std::string> permission)
{
    m_permissions.insert(permission);
}

void AccessGroup::removePermission(std::shared_ptr<std::string> permission)
{
    auto p = m_permissions.find(permission);

    if(p != m_permissions.end())
    {
        m_permissions.erase(p);
    }
}

void PlayerRole::attachGroup(std::shared_ptr<AccessGroup> group)
{
    m_groups.push_back(group);
}
void PlayerRole::removeGroup(std::shared_ptr<AccessGroup> group)
{
    auto iter = m_groups.begin();
    while (iter != m_groups.end())
    {
        if (*iter == group)
        {
            m_groups.erase(iter);
            break;
        }
        ++iter;
    }
}
void PlayerRole::attachPermission(std::shared_ptr<std::string> permission)
{
    m_permissions.insert(permission);
}
void PlayerRole::removePermission(std::shared_ptr<std::string> permission)
{
    auto p = m_permissions.find(permission);

    if(p != m_permissions.end())
    {
        m_permissions.erase(p);
    }
}

bool PlayerRole::hasAccess(std::string_view permission)
{
    // create union of permissions from all groups
    // if find role in union then true
    // m_permissons + sets from groups

    // TODO: if permission contain "group@name" then get access by group name

    std::set<std::shared_ptr<std::string>> temp = m_permissions;

    for(auto group : m_groups)
    {
        auto perms = group->getPerms();
        temp.insert(perms.begin(), perms.end());
    }

    const std::unique_ptr<GroupMngr> &groupMngr = gSPGlobal->getGroupManagerCore();

    std::shared_ptr<std::string> perm = groupMngr->findPermission(permission);

    if(temp.find(perm) != temp.end())
    {
        return true;
    }

    return false;
}

std::size_t GroupMngr::createGroup(std::string_view name)
{
    m_groups.push_back(std::make_shared<AccessGroup>(name));
    return m_groups.size() - 1;
}

std::shared_ptr<std::string> GroupMngr::createPermission(std::string_view name)
{
    for(auto perm : m_permissions)
    {
        if(!perm->compare(name))
        {
            return perm;
        }
    }
    m_permissions.push_back(std::make_shared<std::string>(name));
    return m_permissions[m_permissions.size() - 1];
}

std::shared_ptr<std::string> GroupMngr::findPermission(std::string_view name)
{
    for(auto perm : m_permissions)
    {
        if(!perm->compare(name))
        {
            return perm;
        }
    }
    return nullptr;
}

std::shared_ptr<AccessGroup> GroupMngr::getGroup(std::size_t index)
{
    return m_groups[index];
}

int GroupMngr::findGroup(std::string_view name)
{
    for(std::size_t i = 0; i < m_groups.size(); i++)
    {
        if(!m_groups[i]->getNameCore().compare(name))
        {
            return i;
        }
    }
    return -1;
}

void GroupMngr::clear()
{
    m_groups.clear();
    m_permissions.clear();
}
