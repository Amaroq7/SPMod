/*
 *  Copyright (C) 2018-2021 SPMod Development Team
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

#include "MessageNatives.hpp"
#include "SourcePawnAPI.hpp"
#include "ExtMain.hpp"
#include "StringNatives.hpp"
#include "PlayerNatives.hpp"

#include <ISPGlobal.hpp>
#include <metamod/IMetamod.hpp>
#include <metamod/engine/ILibrary.hpp>

static bool gIgnoreHooks;
static TypeHandler<SPMod::IHookInfo> gMessageHooks;
static SPMod::IMessage *gCurrentMsg;
SPMod::IMessageMngr *gSPMsgMngr;

namespace SPExt
{
    bool initMsgNatives()
    {
        gSPMsgMngr = gSPGlobal->getMessageManager();
        return gSPMsgMngr->isVersionCompatible(SPMod::IMessageMngr::VERSION);
    }
}

// int GetUserMsgId(const char[] msgname);
static cell_t GetUserMsgId(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_msgName = 1
    };

    char *msgName;
    ctx->LocalToString(params[arg_msgName], &msgName);

    if (const Metamod::IMetamod::RegMsg *msgInfo = gMetaAPI->getMsgInfo(msgName); msgInfo)
    {
        return msgInfo->id;
    }

    return 0;
}

// int GetUserMsgName(int msgid, char[] str, int len) = 3 params
static cell_t GetUserMsgName(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_msgType = 1,
        arg_str,
        arg_len
    };

    cell_t msgId = (params[arg_msgType] >= SPMod::MAX_USER_MESSAGES || params[arg_msgType] < 0) ? 0 : params[arg_msgType];

    if (const Metamod::IMetamod::RegMsg *msgInfo = gMetaAPI->getMsgInfo(static_cast<std::uint8_t>(msgId)); msgInfo)
    {
        ctx->StringToLocal(params[arg_str], params[arg_len], msgInfo->name.data());
        if (params[arg_len] >= msgInfo->name.length())
        {
            return msgInfo->name.length();
        }
        else
        {
            return params[arg_len];
        }
    }
    return 0;
}

// native MsgHook HookMessage(int msgtype, MessageHandler handler, HookPriority priority);
static cell_t HookMessage(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_msgType = 1,
        arg_handler,
        arg_priority
    };

    SourcePawn::IPluginFunction *func = ctx->GetFunctionById(params[arg_handler]);

    if (params[arg_msgType] < 0 || static_cast<unsigned int>(params[arg_msgType]) >= SPMod::MAX_USER_MESSAGES)
    {
        ctx->ReportError("Invalid message id (%d).", params[arg_msgType]);
        return -1;
    }

    SPMod::IMessage *msg = gSPMsgMngr->getMessage(
        Metamod::Engine::MsgType(static_cast<std::uint8_t>(params[arg_msgType]))
    );

    // TODO: natives for enable/disable hook

    SPMod::IHookInfo *msgHook = msg->registerHook(
        [func](SPMod::IHook<void, SPMod::IMessage *> *chain, SPMod::IMessage *message) {
            gCurrentMsg = message;

            // typedef MessageHandler = function void (MsgHookChain chain, MessageDest dest, int type, int receiver);
            func->PushCell(reinterpret_cast<intptr_t>(chain));
            func->PushCell(static_cast<cell_t>(message->getDest()));
            func->PushCell(message->getType());
            func->PushCell(message->getEdict()->getIndex());
            func->Execute(nullptr);

            gCurrentMsg = nullptr;
    }, static_cast<SPMod::HookPriority>(params[arg_priority]));

    return gMessageHooks.create(msgHook);
}

// native void UnhookMessage(int msgtype, MsgHook msghook);
static cell_t UnhookMessage(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_msgType = 1,
        arg_msgHook
    };

    if (params[arg_msgType] < 0 || static_cast<ucell_t>(params[arg_msgType]) >= SPMod::MAX_USER_MESSAGES)
    {
        ctx->ReportError("Invalid message id (%d)", params[arg_msgType]);
        return -1;
    }

    SPMod::IMessage *msg = gSPMsgMngr->getMessage(
        Metamod::Engine::MsgType(static_cast<std::uint8_t>(params[arg_msgType]))
    );

    SPMod::IHookInfo *msgHook = gMessageHooks.get(params[arg_msgHook]);
    if (!msgHook)
    {
        ctx->ReportError("Invalid message hook id (%d)", params[arg_msgHook]);
        return 0;
    }

    msg->unregisterHook(msgHook);
    return 1;
}

// native BlockType GetMsgBlock(int msgtype);
static cell_t GetMsgBlock(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_msgType = 1
    };

    if (params[arg_msgType] < 0 || static_cast<ucell_t>(params[arg_msgType]) >= SPMod::MAX_USER_MESSAGES)
    {
        ctx->ReportError("Invalid message id (%d)", params[arg_msgType]);
        return 0;
    }

    SPMod::IMessage *msg = gSPMsgMngr->getMessage(
        Metamod::Engine::MsgType(static_cast<std::uint8_t>(params[arg_msgType]))
    );

    return static_cast<cell_t>(msg->getBlockType());
}

// native void SetMsgBlock(int msgtype, BlockType type);
static cell_t SetMsgBlock(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_msgType = 1,
        arg_blockType
    };

    if (params[arg_msgType] < 0 || static_cast<ucell_t>(params[arg_msgType]) >= SPMod::MAX_USER_MESSAGES)
    {
        ctx->ReportError("Invalid message id (%d)", params[arg_msgType]);
        return 0;
    }

    SPMod::IMessage *msg = gSPMsgMngr->getMessage(
        Metamod::Engine::MsgType(static_cast<std::uint8_t>(params[arg_msgType]))
    );

    msg->setBlockType(static_cast<SPMod::MsgBlockType>(params[arg_blockType]));
    return 1;
}

// native int GetMsgArgs();
static cell_t GetMsgArgs(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params [[maybe_unused]])
{
    if (!gCurrentMsg)
    {
        ctx->ReportError("Invalid function call. Use this native in message hook handler.");
        return 0;
    }

    return static_cast<cell_t>(gCurrentMsg->getParams().size());
}

// native MsgArgType GetMsgArgType(int param);
static cell_t GetMsgArgType(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_index = 1
    };

    if (!gCurrentMsg)
    {
        ctx->ReportError("Invalid function call. Use this native in message hook handler.");
        return 0;
    }

    if (params[arg_index] < 0 || static_cast<std::size_t>(params[arg_index]) >= gCurrentMsg->getParams().size())
    {
        ctx->ReportError("Invalid argument number (%d)", params[arg_index]);
        return 0;
    }

    // TODO: Do this without magic numbers
    cell_t argType = std::visit([](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, std::byte>)
            return 0;
        else if constexpr (std::is_same_v<T, char>)
            return 1;
        else if constexpr (std::is_same_v<T, std::int16_t>)
            return 2;
        else if constexpr (std::is_same_v<T, std::int32_t>)
            return 3;
        else if constexpr (std::is_same_v<T, Metamod::Engine::MsgAngle>)
            return 4;
        else if constexpr (std::is_same_v<T, Metamod::Engine::MsgCoord>)
            return 5;
        else if constexpr (std::is_same_v<T, std::string_view>)
            return 6;

        return 7; // Metamod::Engine::MsgEntity

    }, gCurrentMsg->getParams().at(params[arg_index]));

    return argType;
}

// native int GetMsgArgInt(int param);
static cell_t GetMsgArgInt(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_index = 1
    };

    if (!gCurrentMsg)
    {
        ctx->ReportError("Invalid function call. Use this native in message hook handler.");
        return 0;
    }

    if (params[arg_index] < 0 || static_cast<std::size_t>(params[arg_index]) >= gCurrentMsg->getParams().size())
    {
        ctx->ReportError("Invalid argument number (%d)", params[arg_index]);
        return 0;
    }

    const SPMod::IMessage::Param &param = gCurrentMsg->getParams().at(params[arg_index]);

    if (std::holds_alternative<std::byte>(param))
    {
        return std::to_integer<cell_t>(std::get<std::byte>(param));
    }
    else if (std::holds_alternative<char>(param))
    {
        return std::get<char>(param);
    }
    else if (std::holds_alternative<std::int16_t>(param))
    {
        return std::get<std::int16_t>(param);
    }
    else if (std::holds_alternative<std::int32_t>(param))
    {
        return std::get<std::int32_t>(param);
    }
    else if (std::holds_alternative<Metamod::Engine::MsgEntity>(param))
    {
        return std::get<Metamod::Engine::MsgEntity>(param);
    }
    else
    {
        ctx->ReportError("Invalid type requested");
    }

    return 0;
}

// native float GetMsgArgFloat(int param);
static cell_t GetMsgArgFloat(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_index = 1
    };

    if (!gCurrentMsg)
    {
        ctx->ReportError("Invalid function call. Use this native in message hook handler.");
        return 0;
    }

    if (params[arg_index] < 0 || static_cast<std::size_t>(params[arg_index]) >= gCurrentMsg->getParams().size())
    {
        ctx->ReportError("Invalid argument number (%d)", params[arg_index]);
        return 0;
    }

    const SPMod::IMessage::Param &param = gCurrentMsg->getParams().at(params[arg_index]);

    if (std::holds_alternative<Metamod::Engine::MsgCoord>(param))
    {
        return sp_ftoc(std::get<Metamod::Engine::MsgCoord>(param));
    }
    else if (std::holds_alternative<Metamod::Engine::MsgAngle>(param))
    {
        return sp_ftoc(std::get<Metamod::Engine::MsgAngle>(param));
    }
    else
    {
        ctx->ReportError("Invalid type requested");
    }

    return 0;
}

// native int GetMsgArgString(int param, char[] buffer, int size);
static cell_t GetMsgArgString(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_index = 1,
        arg_buffer,
        arg_size
    };

    if (!gCurrentMsg)
    {
        ctx->ReportError("Invalid function call. Use this native in message hook handler.");
        return 0;
    }

    if (params[arg_index] < 0 || static_cast<std::size_t>(params[arg_index]) >= gCurrentMsg->getParams().size())
    {
        ctx->ReportError("Invalid argument number(%d).", params[arg_index]);
        return 0;
    }

    const SPMod::IMessage::Param &param = gCurrentMsg->getParams().at(params[arg_index]);

    if (std::holds_alternative<std::string>(param))
    {
        char *buffer;
        ctx->LocalToString(params[arg_buffer], &buffer);

        const auto &strValue = std::get<std::string>(param);

        return gSPUtils->strCopy(buffer, params[arg_size], strValue);
    }
    else
    {
        ctx->ReportError("Invalid type requested");
    }

    return 0;
}

// native void SetMsgArgInt(int param, int value);
static cell_t SetMsgArgInt(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_index = 1,
        arg_value
    };

    if (!gCurrentMsg)
    {
        ctx->ReportError("Invalid function call. Use this native in message hook handler.");
        return 0;
    }

    if (params[arg_index] < 0 || static_cast<std::size_t>(params[arg_index]) >= gCurrentMsg->getParams().size())
    {
        ctx->ReportError("Invalid argument number (%d)", params[arg_index]);
        return 0;
    }

    SPMod::IMessage::Param &param = gCurrentMsg->getParams().at(params[arg_index]);

    if (std::holds_alternative<std::byte>(param))
    {
        param = static_cast<std::byte>(params[arg_value]);
    }
    else if (std::holds_alternative<char>(param))
    {
        param = static_cast<char>(params[arg_value]);
    }
    else if (std::holds_alternative<std::int16_t>(param))
    {
        param = static_cast<std::int16_t>(params[arg_value]);
    }
    else if (std::holds_alternative<std::int32_t>(param))
    {
        param = static_cast<std::int32_t>(params[arg_value]);
    }
    else if (std::holds_alternative<Metamod::Engine::MsgEntity>(param))
    {
        param = Metamod::Engine::MsgEntity(static_cast<std::int16_t>(params[arg_value]));
    }
    else
    {
        ctx->ReportError("Invalid type requested");
    }

    return 1;
}

// native void SetMsgArgFloat(int param, float value);
static cell_t SetMsgArgFloat(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_index = 1,
        arg_value
    };

    if (!gCurrentMsg)
    {
        ctx->ReportError("Invalid function call. Use this native in message hook handler.");
        return 0;
    }

    if (params[arg_index] < 0 || static_cast<std::size_t>(params[arg_index]) >= gCurrentMsg->getParams().size())
    {
        ctx->ReportError("Invalid argument number(%d).", params[arg_index]);
        return 0;
    }

    SPMod::IMessage::Param &param = gCurrentMsg->getParams().at(params[arg_index]);

    if (std::holds_alternative<Metamod::Engine::MsgCoord>(param))
    {
        param = Metamod::Engine::MsgCoord(sp_ctof(params[arg_value]));
    }
    else if (std::holds_alternative<Metamod::Engine::MsgAngle>(param))
    {
        param = Metamod::Engine::MsgAngle(sp_ctof(params[arg_value]));
    }
    else
    {
        ctx->ReportError("Invalid type requested");
    }

    return 1;
}

// native void SetMsgArgString(int param, char[] string);
static cell_t SetMsgArgString(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_index = 1,
        arg_string
    };

    if (!gCurrentMsg)
    {
        ctx->ReportError("Invalid function call. Use this native in message hook handler.");
        return 0;
    }

    if (params[arg_index] < 0 || static_cast<std::size_t>(params[arg_index]) >= gCurrentMsg->getParams().size())
    {
        ctx->ReportError("Invalid argument number (%d)", params[arg_index]);
        return 0;
    }

    SPMod::IMessage::Param &param = gCurrentMsg->getParams().at(params[arg_index]);

    if (std::holds_alternative<std::string>(param))
    {
        char *string;
        ctx->LocalToString(params[arg_string], &string);

        std::get<std::string>(param) = string;
    }
    else
    {
        ctx->ReportError("Invalid type requested");
    }

    return 1;
}

// native void MessageBegin(int dest, int msgid, const int origin[3] = {0,0,0}, int player = 0);
static cell_t MessageBegin(SourcePawn::IPluginContext *ctx, const cell_t *params)
{
    enum
    {
        arg_dest = 1,
        arg_msgType,
        arg_origin,
        arg_player,
        arg_ignore_hooks
    };

    auto paramsNum = static_cast<std::size_t>(*params);
    float vecOrigin[3];
    cell_t *cpOrigin;

    if (params[arg_msgType] < 1 || static_cast<unsigned int>(params[arg_msgType]) >= SPMod::MAX_USER_MESSAGES)
    {
        ctx->ReportError("Plugin called MessageBegin with an invalid message id (%d)", params[arg_msgType]);
        return 0;
    }

    gIgnoreHooks = static_cast<bool>(params[arg_ignore_hooks]);
    auto messageDest = static_cast<Metamod::Engine::MsgDest>(params[arg_dest]);
    auto messageType = Metamod::Engine::MsgType(static_cast<std::uint8_t>(params[arg_msgType]));

    switch (messageDest)
    {
        case Metamod::Engine::MsgDest::Broadcast:
        case Metamod::Engine::MsgDest::All:
        case Metamod::Engine::MsgDest::Spec:
        case Metamod::Engine::MsgDest::Init:
            if (gIgnoreHooks)
                gEngine->messageBegin(messageDest, messageType, nullptr, nullptr, Metamod::FuncCallType::Direct);
            else
                gEngine->messageBegin(messageDest, messageType, nullptr, nullptr, Metamod::FuncCallType::Hooks);
            break;
        case Metamod::Engine::MsgDest::Pvs:
        case Metamod::Engine::MsgDest::Pas:
        case Metamod::Engine::MsgDest::PvsR:
        case Metamod::Engine::MsgDest::PasR:
            if (paramsNum < 3)
            {
                ctx->ReportError("Invalid number of parameters passed");
                return 0;
            }

            ctx->LocalToPhysAddr(params[arg_origin], &cpOrigin);

            vecOrigin[0] = sp_ctof(*cpOrigin);
            vecOrigin[1] = sp_ctof(*(cpOrigin + 1));
            vecOrigin[2] = sp_ctof(*(cpOrigin + 2));

            if (gIgnoreHooks)
                gEngine->messageBegin(messageDest, messageType, vecOrigin, nullptr, Metamod::FuncCallType::Direct);
            else
                gEngine->messageBegin(messageDest, messageType, vecOrigin, nullptr, Metamod::FuncCallType::Hooks);

            break;
        case Metamod::Engine::MsgDest::OneUnreliable:
        case Metamod::Engine::MsgDest::One:
            if (paramsNum < 4)
            {
                ctx->ReportError("Invalid number of parameters passed");
                return 0;
            }

            SPMod::IPlayer *player = gSPGlobal->getPlayerManager()->getPlayer(params[arg_player]);

            if (gIgnoreHooks)
                gEngine->messageBegin(messageDest, messageType, nullptr, player->edict(), Metamod::FuncCallType::Direct);
            else
                gEngine->messageBegin(messageDest, messageType, nullptr, player->edict(), Metamod::FuncCallType::Hooks);

            break;
    }

    return 1;
}

// native void MessageEnd();
static cell_t MessageEnd(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params [[maybe_unused]])
{
    if (gIgnoreHooks)
        gEngine->messageEnd(Metamod::FuncCallType::Direct);
    else
        gEngine->messageEnd(Metamod::FuncCallType::Hooks);

    return 1;
}

// native void WriteByte(int value);
static cell_t WriteByte(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_value = 1
    };

    if (gIgnoreHooks)
        gEngine->writeByte(std::byte(params[arg_value]), Metamod::FuncCallType::Direct);
    else
        gEngine->writeByte(std::byte(params[arg_value]), Metamod::FuncCallType::Hooks);

    return 1;
}

// native void WriteChar(char value);
static cell_t WriteChar(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_value = 1
    };

    if (gIgnoreHooks)
        gEngine->writeChar(static_cast<char>(params[arg_value]), Metamod::FuncCallType::Direct);
    else
        gEngine->writeChar(static_cast<char>(params[arg_value]), Metamod::FuncCallType::Hooks);

    return 1;
}

// native void WriteShort(int value);
static cell_t WriteShort(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_value = 1
    };

    if (gIgnoreHooks)
        gEngine->writeShort(static_cast<std::int16_t>(params[arg_value]), Metamod::FuncCallType::Direct);
    else
        gEngine->writeShort(static_cast<std::int16_t>(params[arg_value]), Metamod::FuncCallType::Hooks);

    return 1;
}
// native void WriteLong(int value);
static cell_t WriteLong(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_value = 1
    };

    if (gIgnoreHooks)
        gEngine->writeLong(static_cast<std::int32_t>(params[arg_value]), Metamod::FuncCallType::Direct);
    else
        gEngine->writeLong(static_cast<std::int32_t>(params[arg_value]), Metamod::FuncCallType::Hooks);

    return 1;
}
// native void WriteEntity(int value);
static cell_t WriteEntity(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_value = 1
    };

    if (gIgnoreHooks)
        gEngine->writeEntity(static_cast<std::int16_t>(params[arg_value]), Metamod::FuncCallType::Direct);
    else
        gEngine->writeEntity(static_cast<std::int16_t>(params[arg_value]), Metamod::FuncCallType::Hooks);

    return 1;
}

// native void WriteAngle(float value);
static cell_t WriteAngle(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_value = 1
    };

    if (gIgnoreHooks)
        gEngine->writeAngle(sp_ctof(params[arg_value]), Metamod::FuncCallType::Direct);
    else
        gEngine->writeAngle(sp_ctof(params[arg_value]), Metamod::FuncCallType::Hooks);

    return 1;
}

// native void WriteCoord(float value);
static cell_t WriteCoord(SourcePawn::IPluginContext *ctx [[maybe_unused]], const cell_t *params)
{
    enum
    {
        arg_value = 1
    };

    if (gIgnoreHooks)
        gEngine->writeCoord(sp_ctof(params[arg_value]), Metamod::FuncCallType::Direct);
    else
        gEngine->writeCoord(sp_ctof(params[arg_value]), Metamod::FuncCallType::Hooks);

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

    if (gIgnoreHooks)
        gEngine->writeString(string, Metamod::FuncCallType::Direct);
    else
        gEngine->writeString(string, Metamod::FuncCallType::Hooks);

    return 1;
}

sp_nativeinfo_t gMessageNatives[] = {{"GetUserMsgId", GetUserMsgId},
                                     {"GetUserMsgName", GetUserMsgName},

                                     {"HookMessage", HookMessage},
                                     {"UnhookMessage", UnhookMessage},

                                     {"GetMsgBlock", GetMsgBlock},
                                     {"SetMsgBlock", SetMsgBlock},

                                     {"GetMsgArgs", GetMsgArgs},
                                     {"GetMsgArgType", GetMsgArgType},
                                     {"GetMsgArgInt", GetMsgArgInt},
                                     {"GetMsgArgFloat", GetMsgArgFloat},
                                     {"GetMsgArgString", GetMsgArgString},
                                     {"SetMsgArgInt", SetMsgArgInt},
                                     {"SetMsgArgFloat", SetMsgArgFloat},
                                     {"SetMsgArgString", SetMsgArgString},

                                     {"MessageBegin", MessageBegin},
                                     {"MessageEnd", MessageEnd},
                                     {"WriteByte", WriteByte},
                                     {"WriteChar", WriteChar},
                                     {"WriteShort", WriteShort},
                                     {"WriteLong", WriteLong},
                                     {"WriteEntity", WriteEntity},
                                     {"WriteAngle", WriteAngle},
                                     {"WriteCoord", WriteCoord},
                                     {"WriteString", WriteString},

                                     {nullptr, nullptr}};
