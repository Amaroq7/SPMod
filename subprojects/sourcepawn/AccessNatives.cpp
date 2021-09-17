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

#include "AccessNatives.hpp"
#include "SourcePawnAPI.hpp"

#include <ISPGlobal.hpp>

TypeHandler<SPMod::IAccessGroup> gAccessGroupHandlers;

namespace SPExt
{
    bool initAccessNatives()
    {
        gAccessGroupMngr = gSPGlobal->getGroupAccessManager();
        if (gAccessGroupMngr.expired())
        {
            return false;
        }

        return gAccessGroupMngr.lock()->isVersionCompatible(SPMod::IGroupMngr::VERSION);
    }
}

// Group Group(const char[] name);
static cell_t CreateGroup(SourcePawn::IPluginContext *ctx,
                          const cell_t *params)
{
    enum { arg_name = 1 };

    if (gAccessGroupMngr.expired())
    {
        return static_cast<cell_t>(-1);
    }

    char *name;
    ctx->LocalToString(params[arg_name], &name);

    std::weak_ptr<SPMod::IAccessGroup> accessGroup = gAccessGroupMngr.lock()->createGroup(name);

    if (accessGroup.expired())
    {
        return static_cast<cell_t>(-1);
    }

    return static_cast<cell_t>(gAccessGroupHandlers.create(accessGroup));
}

// native bool Group.AttachPermission(const char[] permission);
static cell_t GroupAttachPermission(SourcePawn::IPluginContext *ctx,
                                    const cell_t *params)
{
    enum { arg_group = 1, arg_perm };

    std::weak_ptr<SPMod::IAccessGroup> group = gAccessGroupHandlers.get(params[arg_group]);
    if (group.expired())
    {
        return 0;
    }

    char *perm;
    ctx->LocalToString(params[arg_perm], &perm);

    group.lock()->addPermission(perm);

    return 1;
}

// native void Group.RemovePermission(const char[] permission);
static cell_t GroupRemovePermission(SourcePawn::IPluginContext *ctx,
                                    const cell_t *params)
{
   enum { arg_group = 1, arg_perm };

   std::weak_ptr<SPMod::IAccessGroup> group = gAccessGroupHandlers.get(params[arg_group]);
   if (group.expired())
   {
       return 0;
   }

   char *perm;
   ctx->LocalToString(params[arg_perm], &perm);

   group.lock()->removePermission(perm);

   return 1;
}

// native Group FindGroup(const char[] group);
static cell_t FindGroup(SourcePawn::IPluginContext *ctx,
                        const cell_t *params)
{
   enum { arg_group = 1 };

   if (gAccessGroupMngr.expired())
   {
       return static_cast<cell_t>(-1);
   }

   char *groupName;
   ctx->LocalToString(params[arg_group], &groupName);

   TypeHandler<SPMod::IAccessGroup>::HandleId groupHandleId = 0;
   for (const auto &groupHandler : gAccessGroupHandlers)
   {
       if (!groupHandler.expired() && groupHandler.lock()->getName() == groupName)
       {
            return static_cast<cell_t>(groupHandleId);
       }
       groupHandleId++;
   }

   std::weak_ptr<SPMod::IAccessGroup> group = gAccessGroupMngr.lock()->getGroup(groupName);
   if (group.expired())
   {
       return static_cast<cell_t>(-1);
   }

   return static_cast<cell_t>(gAccessGroupHandlers.create(group));
}

sp_nativeinfo_t gAccessNatives[] =
{
       {   "Group.Group",              CreateGroup             },
       {   "Group.AttachPermission",   GroupAttachPermission   },
       {   "Group.RemovePermission",   GroupRemovePermission   },

       {   "FindGroup",                FindGroup               },

       {   nullptr,                    nullptr                 }
};