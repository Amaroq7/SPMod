/*
 *  Copyright (C) 2018-2020 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  SPMod is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with SPMod.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "SPGlobal.hpp"
#include "EngineHooks.hpp"
#include "MetaInit.hpp"

#include <metamodcpp_sdk/engine/IHooks.hpp>

void installEngineHooks()
{
    using namespace SPMod;
    static Metamod::Engine::IHooks *hooks = gMetaAPI->getEngine()->getHooks();
    static PlayerMngr *playerMngr = gSPGlobal->getPlayerManager();
    static MessageMngr *msgMngr = gSPGlobal->getMessageManager();

    hooks->messageBegin()->registerHook(
        [](Metamod::Engine::IMessageBeginHook *hook,
                 Metamod::Engine::MsgDest msgDest,
                 Metamod::Engine::MsgType msgType,
                 const float *pOrigin,
                 Metamod::Engine::IEdict *pEdict)
        {
            if (msgType == gSPGlobal->getUserMsgId(UserMsgId::ShowMenu) ||
                msgType == gSPGlobal->getUserMsgId(UserMsgId::VGUIMenu))
            {
                auto player = playerMngr->getPlayer(pEdict);
                player->closeMenu();
            }

            bool shouldPass = msgMngr->MessageBegin(msgDest, msgType, pOrigin, pEdict);
            if (shouldPass)
            {
                hook->callNext(msgDest, msgType, pOrigin, pEdict);
            }
        }
    );

    hooks->messageEnd()->registerHook(
        [](Metamod::Engine::IMessageEndHook *hook)
        {
            bool shouldPass = msgMngr->MessageEnd();
            if (shouldPass)
            {
                hook->callNext();
            }
        }
    );

    hooks->writeByte()->registerHook(
        [](Metamod::Engine::IWriteByteHook *hook, std::byte byteArg)
        {
            bool shouldPass = msgMngr->WriteParam(byteArg);
            if (shouldPass)
            {
                hook->callNext(byteArg);
            }
        }
    );

    hooks->writeChar()->registerHook(
        [](Metamod::Engine::IWriteCharHook *hook, char charArg)
        {
            bool shouldPass = msgMngr->WriteParam(charArg);
            if (shouldPass)
            {
                hook->callNext(charArg);
            }
        }
    );

    hooks->writeShort()->registerHook(
        [](Metamod::Engine::IWriteShortHook *hook, std::int16_t shortArg)
        {
            bool shouldPass = msgMngr->WriteParam(shortArg);
            if (shouldPass)
            {
                hook->callNext(shortArg);
            }
        }
    );

    hooks->writeLong()->registerHook(
        [](Metamod::Engine::IWriteLongHook *hook, std::int32_t longArg)
        {
            bool shouldPass = msgMngr->WriteParam(longArg);
            if (shouldPass)
            {
                hook->callNext(longArg);
            }
        }
    );

    hooks->writeAngle()->registerHook(
        [](Metamod::Engine::IWriteAngleHook *hook, Metamod::Engine::MsgAngle angleArg)
        {
            bool shouldPass = msgMngr->WriteParam(angleArg);
            if (shouldPass)
            {
                hook->callNext(angleArg);
            }
        }
    );

    hooks->writeCoord()->registerHook(
        [](Metamod::Engine::IWriteCoordHook *hook, Metamod::Engine::MsgCoord coordArg)
        {
            bool shouldPass = msgMngr->WriteParam(coordArg);
            if (shouldPass)
            {
                hook->callNext(coordArg);
            }
        }
    );

    hooks->writeString()->registerHook(
        [](Metamod::Engine::IWriteStringHook *hook, std::string_view strArg)
        {
            bool shouldPass = msgMngr->WriteParam(std::string(strArg));
            if (shouldPass)
            {
                hook->callNext(strArg);
            }
        }
    );

    hooks->writeEntity()->registerHook(
        [](Metamod::Engine::IWriteEntityHook *hook, Metamod::Engine::MsgEntity entArg)
        {
            bool shouldPass = msgMngr->WriteParam(entArg);
            if (shouldPass)
            {
                hook->callNext(entArg);
            }
        }
    );

    hooks->regUserMsg()->registerHook(
        [](Metamod::Engine::IRegUserMsgHook *hook, std::string_view name, std::int16_t size)
        {
            Metamod::Engine::MsgType msgType = hook->callNext(name, size);

            if (name == "ShowMenu")
            {
                gSPGlobal->setUserMsgId(UserMsgId::ShowMenu, msgType);
            }
            else if (name == "VGUIMenu")
            {
                gSPGlobal->setUserMsgId(UserMsgId::VGUIMenu, msgType);
            }
            return msgType;
        }
    );
}