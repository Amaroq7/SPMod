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

#include "AccessSystem.hpp"
#include "SPGlobal.hpp"

AccessGroup::AccessGroup(std::string_view name) : m_name(name)
{}

AccessGroup &AccessGroup::operator=(const AccessGroup &other)
{
    m_permissions = other.m_permissions;
    return *this;
}

std::string_view AccessGroup::getName() const
{
    return m_name;
}

bool AccessGroup::addPermission(std::string_view permission)
{
    return m_permissions.emplace(permission).second;
}

void AccessGroup::removePermission(std::string_view permission)
{
   if (auto iter = m_permissions.find(permission.data()); iter != m_permissions.end())
   {
       m_permissions.erase(iter);
   }
}

bool AccessGroup::hasPermission(std::string_view permission)
{
    return m_permissions.find(permission.data()) != m_permissions.end();
}

const std::unordered_set<std::string> &AccessGroup::getPerms() const
{
    return m_permissions;
}

std::weak_ptr<IAccessGroup> GroupMngr::createGroup(std::string_view name)
{
    const auto &[iter, result] = m_groups.emplace(name.data(), std::make_shared<AccessGroup>(name));
    return result ? iter->second : std::weak_ptr<IAccessGroup>();
}

std::weak_ptr<IAccessGroup> GroupMngr::getGroup(std::string_view name)
{
   if (auto iter = m_groups.find(name.data()); iter != m_groups.end())
   {
       return iter->second;
   }

   return {};
}

void GroupMngr::removeGroup(std::weak_ptr<IAccessGroup> group)
{
    if (auto groupToRemove = group.lock(); groupToRemove)
    {
        m_groups.erase(groupToRemove->getName().data());

        PlayerMngr *plrMngr = gSPGlobal->getPlayerManager();
        for (std::uint32_t i = 1; i <= plrMngr->getMaxClients(); i++)
        {
            Player *player = plrMngr->getPlayer(i);
            player->removeGroup(group);
        }
    }
}

std::weak_ptr<IAccessGroup> GroupMngr::cloneGroup(std::weak_ptr<IAccessGroup> group, std::string_view newname)
{
    if (group.expired())
    {
        return {};
    }

    if (auto iter = m_groups.find(group.lock()->getName().data()); iter != m_groups.end())
    {
        return {};
    }

    auto oldGroup = std::dynamic_pointer_cast<AccessGroup>(group.lock());
    if (!oldGroup)
    {
        return {};
    }

    auto newGroup = std::make_shared<AccessGroup>(newname.data());
    *newGroup = *oldGroup;

    const auto &[iter, result] = m_groups.emplace(newname.data(), newGroup);
    return result ? iter->second : std::weak_ptr<IAccessGroup>();
}
