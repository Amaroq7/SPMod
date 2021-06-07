/*
 *  Copyright (C) 2020-2021 Metamod++ Development Team
 *
 *  This file is part of Metamod++.
 *
 *  Metamod++ is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  Metamod++ is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with Metamod++.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "Common.hpp"
#include "../IHookChains.hpp"

#include <string_view>

namespace Metamod::Engine
{
    class IEdict;
    class ICvar;
    class IGameClient;

    using IPrecacheModelHook = IHook<PrecacheId, std::string_view>;
    using IPrecacheModelHookRegistry = IHookRegistry<PrecacheId, std::string_view>;

    using IPrecacheSoundHook = IHook<PrecacheId, std::string_view>;
    using IPrecacheSoundHookRegistry = IHookRegistry<PrecacheId, std::string_view>;

    using IPrecacheGenericHook = IHook<PrecacheId, std::string_view>;
    using IPrecacheGenericHookRegistry = IHookRegistry<PrecacheId, std::string_view>;

    using IChangeLevelHook = IHook<void, std::string_view, std::string_view>;
    using IChangeLevelHookRegistry = IHookRegistry<void, std::string_view, std::string_view>;

    using ISrvCmdHook = IHook<void, std::string_view>;
    using ISrvCmdHookRegistry = IHookRegistry<void, std::string_view>;

    using ISrvExecHook = IHook<void>;
    using ISrvExecHookRegistry = IHookRegistry<void>;

    using IRegSrvCmdHook = IHook<void, std::string_view, ServerCmdCallback>;
    using IRegSrvCmdHookRegistry = IHookRegistry<void, std::string_view, ServerCmdCallback>;

    using IMessageBeginHook = IHook<void, MsgDest, MsgType, const float *, IEdict *>;
    using IMessageBeginHookRegistry = IHookRegistry<void, MsgDest, MsgType, const float *, IEdict *>;

    using IMessageEndHook = IHook<void>;
    using IMessageEndHookRegistry = IHookRegistry<void>;

    using IWriteByteHook = IHook<void, std::byte>;
    using IWriteByteHookRegistry = IHookRegistry<void, std::byte>;

    using IWriteCharHook = IHook<void, char>;
    using IWriteCharHookRegistry = IHookRegistry<void, char>;

    using IWriteShortHook = IHook<void, std::int16_t>;
    using IWriteShortHookRegistry = IHookRegistry<void, std::int16_t>;

    using IWriteLongHook = IHook<void, std::int32_t>;
    using IWriteLongHookRegistry = IHookRegistry<void, std::int32_t>;

    using IWriteEntityHook = IHook<void, MsgEntity>;
    using IWriteEntityHookRegistry = IHookRegistry<void, MsgEntity>;

    using IWriteAngleHook = IHook<void, MsgAngle>;
    using IWriteAngleHookRegistry = IHookRegistry<void, MsgAngle>;

    using IWriteCoordHook = IHook<void, MsgCoord>;
    using IWriteCoordHookRegistry = IHookRegistry<void, MsgCoord>;

    using IWriteStringHook = IHook<void, std::string_view>;
    using IWriteStringHookRegistry = IHookRegistry<void, std::string_view>;

    using IRegUserMsgHook = IHook<MsgType, std::string_view, std::int16_t>;
    using IRegUserMsgHookRegistry = IHookRegistry<MsgType, std::string_view, std::int16_t>;
    
    using IGetPlayerAuthIDHook = IHook<std::string_view, IEdict *>;
    using IGetPlayerAuthIDHookRegistry = IHookRegistry<std::string_view, IEdict *>;
    
    using IGetPlayerUserIDHook = IHook<UserID, IEdict *>;
    using IGetPlayerUserIDHookRegistry = IHookRegistry<UserID, IEdict *>;

    using ISVDropClientHook = IHook<void, IGameClient *, bool, std::string_view>;
    using ISVDropClientHookRegistry = IHookRegistry<void, IGameClient *, bool, std::string_view>;

    using ICvarDirectSetHook = IHook<void, ICvar *, std::string_view>;
    using ICvarDirectSetHookRegistry = IHookRegistry<void, ICvar *, std::string_view>;
    
    using IInfoKeyValueHook = IHook<std::string_view, InfoBuffer, std::string_view>;
    using IInfoKeyValueHookRegistry = IHookRegistry<std::string_view, InfoBuffer, std::string_view>;
    
    using ICmdArgvHook = IHook<std::string_view, std::uint8_t>;
    using ICmdArgvHookRegistry = IHookRegistry<std::string_view, std::uint8_t>;
    
    using ICmdArgsHook = IHook<std::string_view>;
    using ICmdArgsHookRegistry = IHookRegistry<std::string_view>;
    
    using ICmdArgcHook = IHook<std::uint8_t>;
    using ICmdArgcHookRegistry = IHookRegistry<std::uint8_t>;

    using IRegisterCvarHook = IHook<void, std::string_view, std::string_view>;
    using IRegisterCvarHookRegistry = IHookRegistry<void, std::string_view, std::string_view>;

    using IGetCvarHook = IHook<ICvar *, std::string_view>;
    using IGetCvarHookRegistry = IHookRegistry<ICvar *, std::string_view>;

    using ISetModelHook = IHook<void, IEdict *, std::string_view>;
    using ISetModelHookRegistry = IHookRegistry<void, IEdict *, std::string_view>;

    using ICreateEntityHook = IHook<IEdict *>;
    using ICreateEntityHookRegistry = IHookRegistry<IEdict*>;

    using IRemoveEntityHook = IHook<void, IEdict *>;
    using IRemoveEntityHookRegistry = IHookRegistry<void, IEdict *>;

    using IAlertHook = IHook<void, AlertType, std::string_view>;
    using IAlertHookRegistry = IHookRegistry<void, AlertType, std::string_view>;

    using IServerPrintHook = IHook<void, std::string_view>;
    using IServerPrintHookRegistry = IHookRegistry<void, std::string_view>;

    class IHooks
    {
    public:
        virtual ~IHooks() = default;

        virtual IPrecacheModelHookRegistry *precacheModel() = 0;
        virtual IPrecacheSoundHookRegistry *precacheSound() = 0;
        virtual IPrecacheGenericHookRegistry *precacheGeneric() = 0;
        virtual IChangeLevelHookRegistry *changeLevel() = 0;
        virtual ISrvCmdHookRegistry *srvCmd() = 0;
        virtual ISrvExecHookRegistry *srvExec() = 0;
        virtual IRegSrvCmdHookRegistry *regSrvCmd() = 0;
        virtual IMessageBeginHookRegistry *messageBegin() = 0;
        virtual IMessageEndHookRegistry *messageEnd() = 0;
        virtual IWriteByteHookRegistry *writeByte() = 0;
        virtual IWriteCharHookRegistry *writeChar() = 0;
        virtual IWriteShortHookRegistry *writeShort() = 0;
        virtual IWriteLongHookRegistry *writeLong() = 0;
        virtual IWriteEntityHookRegistry *writeEntity() = 0;
        virtual IWriteAngleHookRegistry *writeAngle() = 0;
        virtual IWriteCoordHookRegistry *writeCoord() = 0;
        virtual IWriteStringHookRegistry *writeString() = 0;
        virtual IRegUserMsgHookRegistry *regUserMsg() = 0;
        virtual IGetPlayerAuthIDHookRegistry *getPlayerAuthID() = 0;
        virtual IGetPlayerUserIDHookRegistry *getPlayerUserID() = 0;
        virtual ISVDropClientHookRegistry *svDropClient() = 0;
        virtual ICvarDirectSetHookRegistry *cvarDirectSet() = 0;
        virtual IInfoKeyValueHookRegistry *infoKeyValue() = 0;
        virtual ICmdArgvHookRegistry *cmdArgv() = 0;
        virtual ICmdArgsHookRegistry *cmdArgs() = 0;
        virtual ICmdArgcHookRegistry *cmdArgc() = 0;
        virtual IRegisterCvarHookRegistry *registerCvar() = 0;
        virtual IGetCvarHookRegistry *getCvar() = 0;
        virtual ISetModelHookRegistry *setModel() = 0;
        virtual ICreateEntityHookRegistry *createEntity() = 0;
        virtual IRemoveEntityHookRegistry *removeEntity() = 0;
        virtual IAlertHookRegistry *alert() = 0;
        virtual IServerPrintHookRegistry *serverPrint() = 0;
    };
}
