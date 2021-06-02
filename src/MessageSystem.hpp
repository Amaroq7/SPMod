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

#pragma once

#include "HookChains.hpp"

#include <IMessageSystem.hpp>
#include <public/engine/Common.hpp>
#include <public/engine/IEdict.hpp>

using namespace SPMod;

class Message final : public IMessage
{
public:
    Message() = default;
    ~Message() final = default;

    // IMessage
    std::vector<Param> &getParams() final;

    Metamod::Engine::MsgDest getDest() const final;
    Metamod::Engine::MsgType getType() const final;
    const float *getOrigin() const final;
    Metamod::Engine::IEdict *getEdict() const final;

    IHookInfo *registerHook(IMessage::Handler handler, HookPriority hookPriority) final;
    void unregisterHook(IHookInfo *hook) final;

    MsgBlockType getBlockType() const final;
    void setBlockType(MsgBlockType blockType) final;

    // Message
    bool MessageBegin(Metamod::Engine::MsgDest dest, Metamod::Engine::MsgType type, const float *origin, Metamod::Engine::IEdict *edict);
    bool MessageEnd();
    bool addParam(Param &&param);

private:
    enum class HookType : std::uint8_t
    {
        None = 0,
        Normal,
        Block
    };

private:
    Metamod::Engine::MsgDest m_dest;
    Metamod::Engine::MsgType m_type;
    float *m_origin;
    Metamod::Engine::IEdict *m_edict;

    std::vector<Param> m_params;
    MsgBlockType m_blockType = MsgBlockType::Not;
    HookRegistry<void, IMessage *> m_hooks;
    HookType m_hookType = HookType::None;
};

class MessageMngr final : public IMessageMngr
{
public:
    MessageMngr() = default;
    ~MessageMngr() final = default;

    // IMessageMngr
    IMessage *getMessage(Metamod::Engine::MsgType msgType) const final;

    bool MessageBegin(Metamod::Engine::MsgDest msg_dest,
                      Metamod::Engine::MsgType msg_type,
                      const float *pOrigin,
                      Metamod::Engine::IEdict *ed);

    bool MessageEnd();

    template<typename T, typename = std::enable_if_t<
        std::disjunction_v<
            std::is_same<T, std::byte>,
            std::is_same<T, char>,
            std::is_same<T, std::int16_t>,
            std::is_same<T, std::int32_t>,
            std::is_same<T, Metamod::Engine::MsgEntity>,
            std::is_same<T, Metamod::Engine::MsgCoord>,
            std::is_same<T, Metamod::Engine::MsgAngle>,
            std::is_same<T, std::string_view>
        >
    >>
    bool WriteParam(T value)
    {
        return m_messages[m_currentMsgType]->addParam(value);
    }

private:
    std::array<std::unique_ptr<Message>, MAX_USER_MESSAGES> m_messages;
    Metamod::Engine::MsgType m_currentMsgType = 0;
};
