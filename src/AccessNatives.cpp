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

// Group Group(const char[] name);
static cell_t CreateGroup(SourcePawn::IPluginContext *ctx,
                          const cell_t *params)
{
    enum { arg_name = 1 };

    const std::unique_ptr<GroupMngr> &groupMngr = gSPGlobal->getGroupManagerCore();

    char *name;
    ctx->LocalToString(params[arg_name], &name);
    
    return static_cast<cell_t>(groupMngr->createGroup(name));
}

// native void Group.AttachPermission(const char[] permission);
static cell_t GroupAttachPermission(SourcePawn::IPluginContext *ctx,
                                    const cell_t *params)
{
    enum { arg_group = 1, arg_perm };

    const std::unique_ptr<GroupMngr> &groupMngr = gSPGlobal->getGroupManagerCore();

    char *perm;
    ctx->LocalToString(params[arg_perm], &perm);
    
    std::shared_ptr<AccessGroup> group = groupMngr->getGroup(params[arg_group]);
    std::shared_ptr<std::string> p = groupMngr->createPermission(perm);
    group->addPermisson(p);

    return 1;
}

// native void Group.RemovePermission(const char[] permission);
static cell_t GroupRemovePermission(SourcePawn::IPluginContext *ctx,
                                    const cell_t *params)
{
    enum { arg_group = 1, arg_perm };

    const std::unique_ptr<GroupMngr> &groupMngr = gSPGlobal->getGroupManagerCore();

    char *perm;
    ctx->LocalToString(params[arg_perm], &perm);
    
    std::shared_ptr<AccessGroup> group = groupMngr->getGroup(params[arg_group]);
    std::shared_ptr<std::string> p = groupMngr->createPermission(perm);
    group->removePermission(p);

    return 1;
}

// native Group FindGroup(const char[] group);
static cell_t FindGroup(SourcePawn::IPluginContext *ctx,
                        const cell_t *params)
{
    enum { arg_group = 1 };

    const std::unique_ptr<GroupMngr> &groupMngr = gSPGlobal->getGroupManagerCore();

    char *group;
    ctx->LocalToString(params[arg_group], &group);

    return groupMngr->findGroup(group);
}

sp_nativeinfo_t gAccessNatives[] =
{
    {   "Group.Group",              CreateGroup             },
    {   "Group.AttachPermission",   GroupAttachPermission   },
    {   "Group.RemovePermission",   GroupRemovePermission   },

    {   "FindGroup",                FindGroup               },

    {   nullptr,                    nullptr                 }
};