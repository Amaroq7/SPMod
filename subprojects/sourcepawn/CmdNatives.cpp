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

#include "CmdNatives.hpp"
#include "SourcePawnAPI.hpp"
#include "StringNatives.hpp"

#include <ISPGlobal.hpp>

#include <metamod/engine/ILibrary.hpp>

TypeHandler<SPMod::ICommand> gCommandHandlers;
SPMod::ICommandMngr *gSPCmdMngr;

namespace SPExt
{
    bool initCmdNatives()
    {
        gSPCmdMngr = gSPGlobal->getCommandManager();
        return gSPCmdMngr->isVersionCompatible(SPMod::ICommandMngr::VERSION);
    }
}

// Command(const char[] cmd, ConCallback func, const char[] info = "", bool regex = false, bool server = false,
// const char[] permission = "")
static cell_t CommandCtor(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_cmd = 1,
        arg_func,
        arg_info,
        arg_regex,
        arg_server,
        arg_perm
    };

    SourcePawn::IPluginFunction *func = ctx->GetFunctionById(params[arg_func]);
    char *cmd, *info;
    ctx->LocalToString(params[arg_cmd], &cmd);
    ctx->LocalToString(params[arg_info], &info);

    auto callback = [func](std::shared_ptr<SPMod::IPlayer> player, std::shared_ptr<SPMod::ICommand> cmd) {
        if (func && func->IsRunnable())
        {
            cell_t retValue;
            func->PushCell(static_cast<cell_t>(player ? player->edict()->getIndex() : 0));
            func->PushCell(static_cast<cell_t>(gCommandHandlers.getKey(cmd)));
            func->Execute(&retValue);

            return retValue == 0;
        }

        return true;
    };

    std::weak_ptr<SPMod::ICommand> pCmd;
    if (!params[arg_server])
    {
        char *perm;
        ctx->LocalToString(params[arg_perm], &perm);
        pCmd = gSPCmdMngr->registerCommand(SPMod::ICommand::Type::Client, cmd, info, params[arg_regex],
                                           perm, callback);
    }
    else
    {
        if (params[arg_regex])
        {
            ctx->ReportError("Server command cannot be registered as regex");
            return -1;
        }
        pCmd = gSPCmdMngr->registerCommand(SPMod::ICommand::Type::Server, cmd, info, false, {}, callback);
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

    std::weak_ptr<SPMod::ICommand> pCmd = gCommandHandlers.get(params[arg_id]);
    if (pCmd.expired())
    {
        return 0;
    }

    return static_cast<cell_t>(gSPUtils->strCopy(destBuffer, params[arg_size], pCmd.lock()->getInfo()));
}

// int GetPermission(char[] buffer, int size)
static cell_t GetPermission(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_id = 1,
        arg_buffer,
        arg_size
    };

    std::weak_ptr<SPMod::ICommand> pCmd = gCommandHandlers.get(params[arg_id]);
    if (pCmd.expired())
    {
        return 0;
    }

    char *buffer;
    ctx->LocalToString(params[arg_buffer], &buffer);
    return static_cast<cell_t>(gSPUtils->strCopy(buffer, params[arg_size], pCmd.lock()->getPermission()));
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

    std::string_view argv = gEngine->cmdArgv(params[arg_arg], Metamod::FuncCallType::Direct);
    if (argv.empty())
    {
        return 0;
    }

    return static_cast<cell_t>(gSPUtils->strCopy(destBuffer, params[arg_size], argv));
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

    std::string_view args = gEngine->cmdArgs(Metamod::FuncCallType::Direct);
    if (args.empty())
    {
        return 0;
    }

    return static_cast<cell_t>(gSPUtils->strCopy(destBuffer, params[arg_size], args));
}

// int CmdGetArgsNum()
static cell_t CmdGetArgsNum(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params [[maybe_unused]])
{
    return gEngine->cmdArgc(Metamod::FuncCallType::Direct);
}

sp_nativeinfo_t gCmdsNatives[] = {{"Command.Command", CommandCtor},
                                  {"Command.GetInfo", GetInfo},
                                  {"Command.GetPermission", GetPermission},
                                  {"CmdGetArgv", CmdGetArgv},
                                  {"CmdGetArgs", CmdGetArgs},
                                  {"CmdGetArgsNum", CmdGetArgsNum},
                                  {nullptr, nullptr}};
