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

#include "MessageSystem.hpp"
#include "MetaInit.hpp"

#include <utility>

bool Message::MessageBegin(Metamod::Engine::MsgDest dest, Metamod::Engine::MsgType type, const float *origin, Metamod::Engine::IEdict *edict)
{
    if (m_blockType != MsgBlockType::Not)
    {
        m_hookType = HookType::Block;
        return false;
    }

    if (m_hooks.hasHooks())
    {
        m_hookType = HookType::Normal;

        m_dest = dest;
        m_type = type;
        m_origin = const_cast<float *>(origin);
        m_edict = edict;

        return false;
    }

    return true;
}

bool Message::MessageEnd()
{
    if (m_hookType == HookType::None)
    {
        return true;
    }

    if (m_hookType != HookType::Block)
    {
        if (m_blockType == MsgBlockType::Once)
        {
            m_blockType = MsgBlockType::Not;
            m_hookType = HookType::None;
        }

        return false;
    }

    // add contidions for hooks like in amxx events
    // hooks can change params
    // then exec message

    auto sendMsg = [this](IMessage *) {
        gEngine->messageBegin(m_dest, m_type, m_origin, m_edict, Metamod::FuncCallType::Direct);

        for (const auto &param : m_params)
        {
            std::visit([](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;
                    if constexpr (std::is_same_v<T, std::byte>)
                        gEngine->writeByte(arg, Metamod::FuncCallType::Direct);
                    else if constexpr (std::is_same_v<T, char>)
                        gEngine->writeChar(arg, Metamod::FuncCallType::Direct);
                    else if constexpr (std::is_same_v<T, std::int16_t>)
                        gEngine->writeShort(arg, Metamod::FuncCallType::Direct);
                    else if constexpr (std::is_same_v<T, std::int32_t>)
                        gEngine->writeLong(arg, Metamod::FuncCallType::Direct);
                    else if constexpr (std::is_same_v<T, Metamod::Engine::MsgCoord>)
                        gEngine->writeCoord(arg, Metamod::FuncCallType::Direct);
                    else if constexpr (std::is_same_v<T, Metamod::Engine::MsgEntity>)
                        gEngine->writeEntity(arg, Metamod::FuncCallType::Direct);
                    else if constexpr (std::is_same_v<T, Metamod::Engine::MsgAngle>)
                        gEngine->writeAngle(arg, Metamod::FuncCallType::Direct);
                    else if constexpr (std::is_same_v<T, std::string>)
                        gEngine->writeString(arg, Metamod::FuncCallType::Direct);
            }, param);
        }
        gEngine->messageEnd(Metamod::FuncCallType::Direct);
    };

    m_hooks.callChain(sendMsg, this);
    m_params.clear();
    m_hookType = HookType::None;
    return false;
}

bool Message::addParam(Param &&param)
{
    if (m_hookType == HookType::None)
    {
        return true;
    }

    if (m_hookType == HookType::Normal)
    {
        m_params.emplace_back(param);
    }

    return false;
}

std::vector<Message::Param> &Message::getParams()
{
    return m_params;
}

Metamod::Engine::MsgDest Message::getDest() const
{
    return m_dest;
}

Metamod::Engine::MsgType Message::getType() const
{
    return m_type;
}

const float *Message::getOrigin() const
{
    return m_origin;
}

Metamod::Engine::IEdict *Message::getEdict() const
{
    return m_edict;
}

MsgBlockType Message::getBlockType() const
{
    return m_blockType;
}

void Message::setBlockType(MsgBlockType blockType)
{
    m_blockType = blockType;
}

IHookInfo *Message::registerHook(IMessage::Handler handler, HookPriority hookPriority)
{
    return m_hooks.registerHook(handler, hookPriority);
}

void Message::unregisterHook(IHookInfo *hook)
{
    if (!hook)
    {
        return;
    }

    m_hooks.unregisterHook(hook);
}

MessageMngr::MessageMngr()
{
    for (auto &msg : m_messages)
    {
        msg = std::make_unique<Message>();
    }
}

bool MessageMngr::MessageBegin(Metamod::Engine::MsgDest msg_dest,
                               Metamod::Engine::MsgType msg_type,
                               const float *pOrigin,
                               Metamod::Engine::IEdict *ed)
{
    return m_messages[msg_type]->MessageBegin(msg_dest, msg_type, pOrigin, ed);
}

IMessage *MessageMngr::getMessage(Metamod::Engine::MsgType msgType) const
{
    return m_messages[msgType].get();
}

bool MessageMngr::MessageEnd()
{
    return m_messages[m_currentMsgType]->MessageEnd();
}
