/*
 *  Copyright (C) 2018 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
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

// Command Command(const char[] cmd, ConCallback func, const char[] info = "", bool server = false, int flags = 0)
static cell_t CommandCtor(SourcePawn::IPluginContext *ctx,
                          const cell_t *params)
{
    enum { arg_cmd = 1, arg_func, arg_info, arg_server, arg_flags };

    const std::unique_ptr<CommandMngr> &cmdMngr = gSPGlobal->getCommandManagerCore();
    SourcePawn::IPluginFunction *func = ctx->GetFunctionById(params[arg_func]);
    char *cmd, *info;
    ctx->LocalToString(params[arg_cmd], &cmd);
    ctx->LocalToString(params[arg_info], &info);

    std::shared_ptr<Command> pCmd;
    if (!params[arg_server])
        pCmd = cmdMngr->registerCommand<ClientCommand>(cmd, info, func, params[arg_flags]);
    else
    {
        pCmd = cmdMngr->registerCommand<ServerCommand>(cmd, info, func);
        REG_SVR_COMMAND(cmd, PluginSrvCmd);
    }

    return pCmd->getId();
}

// int GetInfo(char[] buffer, int size)
static cell_t GetInfo(SourcePawn::IPluginContext *ctx,
                      const cell_t *params)
{
    enum { arg_id = 1, arg_buffer, arg_size };

    char *destBuffer;
    ctx->LocalToString(params[arg_buffer], &destBuffer);
    const std::unique_ptr<CommandMngr> &cmdMngr = gSPGlobal->getCommandManagerCore();

    std::shared_ptr<Command> pCmd = cmdMngr->getCommand(params[arg_id]);

    if (!pCmd)
        return 0;

    return gSPGlobal->getUtilsCore()->strCopyCore(destBuffer, params[arg_size], pCmd->getInfo());
}

// property int Access.get()
static cell_t GetAccess(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                        const cell_t *params)
{
    enum { arg_id = 1 };

    const std::unique_ptr<CommandMngr> &cmdMngr = gSPGlobal->getCommandManagerCore();
    std::shared_ptr<Command> pCmd = cmdMngr->getCommand(params[arg_id]);
    if (!pCmd)
        return 0;

    return pCmd->getAccess();
}

// int CmdGetArgv(int arg, char[] buffer, int size)
static cell_t CmdGetArgv(SourcePawn::IPluginContext *ctx,
                         const cell_t *params)
{
    enum { arg_id = 1, arg_buffer, arg_size };

    char *destBuffer;
    ctx->LocalToString(params[arg_buffer], &destBuffer);

    const char *argv = CMD_ARGV(params[arg_id]);

    if (!argv)
        return 0;

    return gSPGlobal->getUtilsCore()->strCopyCore(destBuffer, params[arg_size], argv);
}

// int CmdGetArgs(char[] buffer, int size)
static cell_t CmdGetArgs(SourcePawn::IPluginContext *ctx,
                         const cell_t *params)
{
    enum { arg_buffer = 1, arg_size };

    char *destBuffer;
    ctx->LocalToString(params[arg_buffer], &destBuffer);

    const char *args = CMD_ARGS();

    if (!args)
        return 0;

    return gSPGlobal->getUtilsCore()->strCopyCore(destBuffer, params[arg_size], args);
}

// int CmdGetArgsNum()
static cell_t CmdGetArgsNum(SourcePawn::IPluginContext *ctx [[maybe_unused]],
                            const cell_t *params [[maybe_unused]])
{
    return CMD_ARGC();
}

sp_nativeinfo_t gCmdsNatives[] =
{
    { "Command.Command",    CommandCtor   },
    { "Command.GetInfo",    GetInfo       },
    { "Command.Access.get", GetAccess     },
    { "CmdGetArgv",         CmdGetArgv    },
    { "CmdGetArgs",         CmdGetArgs    },
    { "CmdGetArgsNum",      CmdGetArgsNum },
    { nullptr,              nullptr       }
};
