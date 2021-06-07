/*
 *  Copyright (C) 2020 Metamod++ Development Team
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

#include <string_view>
#include <cinttypes>
#include <cstddef>

#if defined META_CORE
typedef struct edict_s edict_t;
typedef struct entvars_s entvars_t;
typedef struct enginefuncs_s enginefuncs_t;
typedef struct TraceResult TraceResult;
#endif

namespace Metamod::Engine
{
    class IEdict;
    class ITraceResult;
    class IHooks;
    class ICvar;
    class IEntVars;

    class ILibrary
    {
    public:
        virtual ~ILibrary() = default;

        virtual IEdict *getEdict(std::uint32_t index) = 0;
        virtual ITraceResult *createTraceResult() = 0;
        virtual void freeTraceResult(ITraceResult *tr) = 0;
        virtual IHooks *getHooks() = 0;
        [[nodiscard]] virtual std::array<std::uint32_t, 2> getReHLDSVersion() const = 0;

        /* Engine funcs */
        [[nodiscard]] virtual PrecacheId precacheModel(std::string_view model, FuncCallType callType) const = 0;
        [[nodiscard]] virtual PrecacheId precacheSound(std::string_view sound, FuncCallType callType) const = 0;
        [[nodiscard]] virtual PrecacheId precacheGeneric(std::string_view generic, FuncCallType callType) const = 0;
        virtual void changeLevel(std::string_view level1, std::string_view level2, FuncCallType callType) const = 0;
        virtual void serverCommand(std::string_view cmd, FuncCallType callType) const = 0;
        virtual void serverExecute(FuncCallType callType) const = 0;
        virtual void registerSrvCommand(std::string_view cmd, ServerCmdCallback cb, FuncCallType callType) = 0;
        virtual void messageBegin(MsgDest msgDest,
                                  MsgType msgType,
                                  const float *pOrigin,
                                  IEdict *pEdict, FuncCallType callType) const = 0;
        virtual void messageEnd(FuncCallType callType) const = 0;
        virtual void writeByte(std::byte byteArg, FuncCallType callType) const = 0;
        virtual void writeChar(char charArg, FuncCallType callType) const = 0;
        virtual void writeShort(std::int16_t shortArg, FuncCallType callType) const = 0;
        virtual void writeLong(std::int32_t longArg, FuncCallType callType) const = 0;
        virtual void writeEntity(MsgEntity entArg, FuncCallType callType) const = 0;
        virtual void writeAngle(MsgAngle angleArg, FuncCallType callType) const = 0;
        virtual void writeCoord(MsgCoord coordArg, FuncCallType callType) const = 0;
        virtual void writeString(std::string_view strArg, FuncCallType callType) const = 0;
        [[nodiscard]] virtual MsgType regUserMsg(std::string_view name, std::int16_t size, FuncCallType callType) const = 0;
        virtual std::string_view getPlayerAuthID(IEdict *pEdict, FuncCallType callType) const = 0;
        virtual UserID getPlayerUserID(IEdict *pEdict, FuncCallType callType) const = 0;
        [[nodiscard]] virtual std::string_view infoKeyValue(InfoBuffer infobuffer, std::string_view key, FuncCallType callType) const = 0;
        [[nodiscard]] virtual std::string_view cmdArgs(FuncCallType callType) const = 0;
        [[nodiscard]] virtual std::string_view cmdArgv(std::uint8_t argc, FuncCallType callType) const = 0;
        [[nodiscard]] virtual std::uint8_t cmdArgc(FuncCallType callType) const = 0;
        virtual void registerCvar(std::string_view name, std::string_view value, FuncCallType callType) = 0;
        virtual ICvar *getCvar(std::string_view name, FuncCallType callType) = 0;
        virtual void setModel(IEdict *pEdict, std::string_view model, FuncCallType callType) = 0;
        virtual IEdict *createEntity(FuncCallType callType) = 0;
        virtual void removeEntity(IEdict *pEdict, FuncCallType callType) = 0;
        virtual void alert(AlertType alertType, std::string_view msg, FuncCallType callType) = 0;
        virtual void print(std::string_view szMsg, FuncCallType callType) = 0;

        /* Engine globals */
        [[nodiscard]] virtual float getTime() const = 0;
        [[nodiscard]] virtual std::string_view getMapName() const = 0;

        /* ReFuncs */
        virtual bool addExtDll(void *hModule) const = 0;
        virtual void removeExtDll(void *hModule) const = 0;
        virtual void removeCmd(std::string_view cmd_name) = 0;

        /* ReHLDS server data */
        [[nodiscard]] virtual std::uint32_t getWorldmapCrc() const = 0;

#if defined META_CORE
        virtual IEdict *getEdict(edict_t *edict) = 0;
        virtual IEntVars *getEntVars(entvars_t *vars) = 0;
        virtual ITraceResult *createTraceResult(::TraceResult *tr) = 0;
        [[nodiscard]] virtual const enginefuncs_t *getEngineFuncs() const = 0;
        virtual void initializeEdicts(edict_t *pEdictList, std::uint32_t edictCount, std::uint32_t clientMax) = 0;
        [[nodiscard]] virtual edict_t *getEngineEdictList() const = 0;
#endif
    };
} // namespace Metamod::Engine
