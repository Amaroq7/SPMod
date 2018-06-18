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
    size_t bufferSize = params[arg_size];
    size_t writtenChars = bufferSize - 1; //Dont count null

    std::shared_ptr<Command> pCmd = cmdMngr->getCommand(params[arg_id]);

    if (!pCmd)
        return 0;

#if defined __STDC_LIB_EXT1__ || defined SP_MSVC
    #if defined SP_MSVC
    strncpy_s(destBuffer, bufferSize, pCmd->getInfo().data(), _TRUNCATE);
    #else
    strncpy_s(destBuffer, bufferSize, pCmd->getInfo().data(), bufferSize - 1);
    #endif
#else
    std::strncpy(destBuffer, pCmd->getInfo().data(), bufferSize);
    destBuffer[bufferSize - 1] = '\0';
#endif

    if (pCmd->getInfo().length() < writtenChars)
        writtenChars = pCmd->getInfo().length();

    return writtenChars;
}

// property int Access.get()
static cell_t GetAccess(SourcePawn::IPluginContext *ctx,
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
    size_t bufferSize = params[arg_size];
    size_t writtenChars = bufferSize - 1; //Dont count null

    const char *argv = CMD_ARGV(params[arg_id]);

    if (!argv)
        return 0;

#if defined __STDC_LIB_EXT1__ || defined SP_MSVC
    #if defined SP_MSVC
    strncpy_s(destBuffer, bufferSize, argv, _TRUNCATE);
    #else
    strncpy_s(destBuffer, bufferSize, argv, bufferSize - 1);
    #endif
#else
    std::strncpy(destBuffer, argv, bufferSize);
    destBuffer[bufferSize - 1] = '\0';
#endif

    size_t argvLen = strlen(argv);
    if (argvLen < writtenChars)
        writtenChars = argvLen;

    return writtenChars;
}

// int CmdGetArgs(char[] buffer, int size)
static cell_t CmdGetArgs(SourcePawn::IPluginContext *ctx,
                         const cell_t *params)
{
    enum { arg_buffer = 1, arg_size };

    char *destBuffer;
    ctx->LocalToString(params[arg_buffer], &destBuffer);
    size_t bufferSize = params[arg_size];
    size_t writtenChars = bufferSize - 1; //Dont count null

    const char *args = CMD_ARGS();

    if (!args)
        return 0;

#if defined __STDC_LIB_EXT1__ || defined SP_MSVC
    #if defined SP_MSVC
    strncpy_s(destBuffer, bufferSize, args, _TRUNCATE);
    #else
    strncpy_s(destBuffer, bufferSize, args, bufferSize - 1);
    #endif
#else
    std::strncpy(destBuffer, args, bufferSize);
    destBuffer[bufferSize - 1] = '\0';
#endif

    size_t argvLen = strlen(args);
    if (argvLen < writtenChars)
        writtenChars = argvLen;

    return writtenChars;
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
