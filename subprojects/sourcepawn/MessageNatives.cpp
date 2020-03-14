/*
 *  Copyright (C) 2018-2019 SPMod Development Team
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

#define MSG_BROADCAST 0 // unreliable to all
#define MSG_ONE       1 // reliable to one (msg_entity)
#define MSG_ALL       2 // reliable to all
#define MSG_INIT      3 // write to the init string
#define MSG_PVS       4 // Ents in PVS of org
#define MSG_PAS       5 // Ents in PAS of org
#define MSG_PVS_R     6 // Reliable to PVS
#define MSG_PAS_R     7 // Reliable to PAS
#define MSG_ONE_UNRELIABLE                                                                                             \
    8 // Send to one client, but don't put in reliable stream, put in unreliable datagram ( could be dropped )
#define MSG_SPEC 9 // Sends to all spectator proxies

// int GetUserMsgId(const char[] msgname);
static cell_t GetUserMsgId(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_msgname = 1
    };

    char *string;
    ctx->LocalToString(params[arg_msgname], &string);

    return gSPGlobal->getMetaFuncs()->getUsrMsgId(string);
}

// int GetUserMsgName(int msgid, char[] str, int len) = 3 params
static cell_t GetUserMsgName(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_msg = 1,
        arg_str,
        arg_len
    };

    const char *string = gSPGlobal->getMetaFuncs()->getUsrMsgName(params[arg_msg]);
    if (string)
    {
        ctx->StringToLocal(params[arg_str], params[arg_len], string);
        return strlen(string);
    }

    return 0;
}

static cell_t MessageBegin_(SourcePawn::IPluginContext *ctx, const cell_t *params, bool useFloat)
{
    enum
    {
        arg_dest = 1,
        arg_msg_type,
        arg_origin,
        arg_player
    };

    int numparam = *params;
    float vecOrigin[3];
    cell_t *cpOrigin;

    if (params[arg_msg_type] < 1 || ((params[arg_msg_type] > 63) // maximal number of engine messages
                                     && !gSPGlobal->getMetaFuncs()->getUsrMsgName(params[2])))
    {
        ctx->ReportError("Plugin called MessageBegin with an invalid message id (%d).", params[arg_msg_type]);
        return 0;
    }

    switch (params[arg_dest])
    {
        case MSG_BROADCAST:
        case MSG_ALL:
        case MSG_SPEC:
        case MSG_INIT:
            gSPGlobal->getEngineFuncs()->messageBegin(params[arg_dest], params[arg_msg_type], nullptr);
            break;
        case MSG_PVS:
        case MSG_PAS:
        case MSG_PVS_R:
        case MSG_PAS_R:
            if (numparam < 3)
            {
                ctx->ReportError("Invalid number of parameters passed");
                return 0;
            }

            ctx->LocalToPhysAddr(params[arg_origin], &cpOrigin);

            if (!useFloat)
            {
                vecOrigin[0] = static_cast<float>(*cpOrigin);
                vecOrigin[1] = static_cast<float>(*(cpOrigin + 1));
                vecOrigin[2] = static_cast<float>(*(cpOrigin + 2));
            }
            else
            {
                vecOrigin[0] = sp_ctof(*cpOrigin);
                vecOrigin[1] = sp_ctof(*(cpOrigin + 1));
                vecOrigin[2] = sp_ctof(*(cpOrigin + 2));
            }

            gSPGlobal->getEngineFuncs()->messageBegin(params[arg_dest], params[arg_msg_type], vecOrigin);

            break;
        case MSG_ONE_UNRELIABLE:
        case MSG_ONE:
            if (numparam < 4)
            {
                ctx->ReportError("Invalid number of parameters passed");
                return 0;
            }

            SPMod::IPlayer *player = gSPGlobal->getPlayerManager()->getPlayer(params[arg_player]);
            gSPGlobal->getEngineFuncs()->messageBegin(params[arg_dest], params[arg_msg_type], nullptr, player);
            break;
    }

    return 1;
}

// native void MessageBegin(int dest, int msgid, const int origin[3] = {0,0,0}, int player = 0);
static cell_t MessageBegin(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    return MessageBegin_(ctx, params, false);
}

// native void MessageBeginF(int dest, int msgid, const float origin[3] = {0.0,0.0,0.0}, int player = 0);
static cell_t MessageBeginF(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    return MessageBegin_(ctx, params, true);
}

// native void MessageEnd();
static cell_t MessageEnd(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params [[maybe_unused]])
{
    gSPGlobal->getEngineFuncs()->messageEnd();
    return 1;
}

// native void WriteByte(int value);
static cell_t WriteByte(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_value = 1
    };
    gSPGlobal->getEngineFuncs()->writeByte(params[arg_value]);
    return 1;
}
// native void WriteChar(char value);
static cell_t WriteChar(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_value = 1
    };
    gSPGlobal->getEngineFuncs()->writeChar(params[arg_value]);
    return 1;
}
// native void WriteShort(int value);
static cell_t WriteShort(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_value = 1
    };
    gSPGlobal->getEngineFuncs()->writeShort(params[arg_value]);
    return 1;
}
// native void WriteLong(int value);
static cell_t WriteLong(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_value = 1
    };
    gSPGlobal->getEngineFuncs()->writeLong(params[arg_value]);
    return 1;
}
// native void WriteEntity(int value);
static cell_t WriteEntity(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_value = 1
    };
    gSPGlobal->getEngineFuncs()->writeEntity(params[arg_value]);
    return 1;
}
// native void WriteAngle(int value);
static cell_t WriteAngle(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_value = 1
    };
    gSPGlobal->getEngineFuncs()->writeAngle(static_cast<float>(params[arg_value]));
    return 1;
}
// native void WriteAngleF(float value);
static cell_t WriteAngleF(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_value = 1
    };
    gSPGlobal->getEngineFuncs()->writeAngle(sp_ctof(params[arg_value]));
    return 1;
}
// native void WriteCoord(int value);
static cell_t WriteCoord(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_value = 1
    };
    gSPGlobal->getEngineFuncs()->writeCoord(static_cast<float>(params[arg_value]));
    return 1;
}
// native void WriteCoordF(float value);
static cell_t WriteCoordF(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_value = 1
    };
    gSPGlobal->getEngineFuncs()->writeCoord(sp_ctof(params[arg_value]));
    return 1;
}
// native void WriteString(char[] value);
static cell_t WriteString(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_value = 1
    };
    char *string;
    ctx->LocalToString(params[arg_value], &string);
    gSPGlobal->getEngineFuncs()->writeString(string);
    return 1;
}

sp_nativeinfo_t gMessageNatives[] = {{"GetUserMsgId", GetUserMsgId}, {"GetUserMsgName", GetUserMsgName},
                                     {"MessageBegin", MessageBegin}, {"MessageBeginF", MessageBeginF},
                                     {"MessageEnd", MessageEnd},     {"WriteByte", WriteByte},
                                     {"WriteChar", WriteChar},       {"WriteShort", WriteShort},
                                     {"WriteLong", WriteLong},       {"WriteEntity", WriteEntity},
                                     {"WriteAngle", WriteAngle},     {"WriteAngleF", WriteAngleF},
                                     {"WriteCoord", WriteCoord},     {"WriteCoordF", WriteCoordF},
                                     {"WriteString", WriteString},   {nullptr, nullptr}};
