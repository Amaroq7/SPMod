/*
 *  Copyright (C) 2018-2020 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SPMod is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with SPMod.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "ExtMain.hpp"

TypeHandler<SPMod::ICommand> gCommandHandlers;

// Command Command(const char[] cmd, ConCallback func, const char[] info = "", bool server = false, int flags = 0)
static cell_t CommandCtor(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_cmd = 1,
        arg_func,
        arg_info,
        arg_server,
        arg_flags
    };

    SPMod::ICommandMngr *cmdMngr = gSPGlobal->getCommandManager();
    SourcePawn::IPluginFunction *func = ctx->GetFunctionById(params[arg_func]);
    char *cmd, *info;
    ctx->LocalToString(params[arg_cmd], &cmd);
    ctx->LocalToString(params[arg_info], &info);

    SPMod::ICommand *pCmd;
    if (!params[arg_server])
        pCmd = cmdMngr->registerCommand(SPMod::ICommand::Type::Client, cmd, info, params[arg_flags],
                                        SPExt::Listener::CmdCallback, func);
    else
    {
        pCmd =
            cmdMngr->registerCommand(SPMod::ICommand::Type::Server, cmd, info, 0, SPExt::Listener::CmdCallback, func);
        gSPGlobal->getEngineFuncs()->registerSrvCommand(cmd);
    }

    return gCommandHandlers.create(pCmd);
}

// int GetInfo(char[] buffer, int size)
static cell_t GetInfo(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_id = 1,
        arg_buffer,
        arg_size
    };

    char *destBuffer;
    ctx->LocalToString(params[arg_buffer], &destBuffer);

    SPMod::ICommand *pCmd = gCommandHandlers.get(params[arg_id]);
    if (!pCmd)
        return 0;

    return gSPGlobal->getUtils()->strCopy(destBuffer, params[arg_size], pCmd->getInfo());
}

// property int Access.get()
static cell_t GetAccess(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_id = 1
    };

    SPMod::ICommand *pCmd = gCommandHandlers.get(params[arg_id]);
    if (!pCmd)
        return 0;

    return pCmd->getAccess();
}

// int CmdGetArgv(int arg, char[] buffer, int size)
static cell_t CmdGetArgv(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_arg = 1,
        arg_buffer,
        arg_size
    };

    char *destBuffer;
    ctx->LocalToString(params[arg_buffer], &destBuffer);

    std::string_view argv = gSPGlobal->getEngineFuncs()->getArg(params[arg_arg]);
    if (argv.empty())
        return 0;

    return gSPGlobal->getUtils()->strCopy(destBuffer, params[arg_size], argv);
}

// int CmdGetArgs(char[] buffer, int size)
static cell_t CmdGetArgs(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_buffer = 1,
        arg_size
    };

    char *destBuffer;
    ctx->LocalToString(params[arg_buffer], &destBuffer);

    std::string_view args = gSPGlobal->getEngineFuncs()->getArgs();
    if (args.empty())
        return 0;

    return gSPGlobal->getUtils()->strCopy(destBuffer, params[arg_size], args);
}

// int CmdGetArgsNum()
static cell_t CmdGetArgsNum(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params [[maybe_unused]])
{
    return gSPGlobal->getEngineFuncs()->getArgc();
}

sp_nativeinfo_t gCmdsNatives[] = {{"Command.Command", CommandCtor},
                                  {"Command.GetInfo", GetInfo},
                                  {"Command.Access.get", GetAccess},
                                  {"CmdGetArgv", CmdGetArgv},
                                  {"CmdGetArgs", CmdGetArgs},
                                  {"CmdGetArgsNum", CmdGetArgsNum},
                                  {nullptr, nullptr}};
