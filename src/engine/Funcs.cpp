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

#include "../spmod.hpp"

namespace SPMod::Engine
{
    Funcs::Funcs(bool hook) : m_hook(hook) {}

    std::string_view Funcs::getArg(std::uint32_t arg) const
    {
        return (m_hook) ? gpEngineFuncs->pfnCmd_Argv(arg) : CMD_ARGV(arg);
    }

    std::string_view Funcs::getArgs() const
    {
        return (m_hook) ? gpEngineFuncs->pfnCmd_Args() : CMD_ARGS();
    }

    std::uint32_t Funcs::getArgc() const
    {
        return (m_hook) ? gpEngineFuncs->pfnCmd_Argc() : CMD_ARGC();
    }

    void Funcs::serverPrint(std::string_view msg) const
    {
        (m_hook) ? gpEngineFuncs->pfnServerPrint(msg.data()) : SERVER_PRINT(msg.data());
    }

    std::uint32_t Funcs::precacheModel(std::string_view model) const
    {
        return (m_hook) ? gpEngineFuncs->pfnPrecacheModel(model.data()) : PRECACHE_MODEL(model.data());
    }

    std::uint32_t Funcs::precacheSound(std::string_view sound) const
    {
        return (m_hook) ? gpEngineFuncs->pfnPrecacheSound(sound.data()) : PRECACHE_SOUND(sound.data());
    }

    std::uint32_t Funcs::precacheGeneric(std::string_view generic) const
    {
        return (m_hook) ? gpEngineFuncs->pfnPrecacheGeneric(generic.data()) : PRECACHE_GENERIC(generic.data());
    }

    void Funcs::changeLevel(std::string_view level) const
    {
        (m_hook) ? gpEngineFuncs->pfnChangeLevel(level.data(), nullptr) : CHANGE_LEVEL(level.data(), nullptr);
    }

    void Funcs::serverCommand(std::string_view cmd) const
    {
        (m_hook) ? gpEngineFuncs->pfnServerCommand(cmd.data()) : SERVER_COMMAND(cmd.data());
    }

    void Funcs::serverExecute() const
    {
        (m_hook) ? gpEngineFuncs->pfnServerExecute() : SERVER_EXECUTE();
    }

    void Funcs::registerSrvCommand(std::string_view cmd, ServerCmdCallback callback) const
    {
        (m_hook) ? gpEngineFuncs->pfnAddServerCommand(cmd.data(), callback) : REG_SVR_COMMAND(cmd.data(), callback);
    }

    void Funcs::messageBegin(MsgDest msgDest, std::uint32_t msgType, const float *pOrigin, IEdict *pEdict) const
    {
        (m_hook) ? gpEngineFuncs->pfnMessageBegin(static_cast<std::int32_t>(msgDest), msgType, pOrigin,
                                                  INDEXENT(pEdict->getIndex()))
                 : MESSAGE_BEGIN(static_cast<std::int32_t>(msgDest), msgType, pOrigin, INDEXENT(pEdict->getIndex()));
    }

    void Funcs::messageEnd() const
    {
        (m_hook) ? gpEngineFuncs->pfnMessageEnd() : MESSAGE_END();
    }

    void Funcs::writeByte(std::int32_t byteArg) const
    {
        (m_hook) ? gpEngineFuncs->pfnWriteByte(byteArg) : WRITE_BYTE(byteArg);
    }

    void Funcs::writeChar(std::int32_t charArg) const
    {
        (m_hook) ? gpEngineFuncs->pfnWriteChar(charArg) : WRITE_CHAR(charArg);
    }

    void Funcs::writeShort(std::int32_t shortArg) const
    {
        (m_hook) ? gpEngineFuncs->pfnWriteShort(shortArg) : WRITE_SHORT(shortArg);
    }

    void Funcs::writeLong(std::int32_t longArg) const
    {
        (m_hook) ? gpEngineFuncs->pfnWriteLong(longArg) : WRITE_LONG(longArg);
    }

    void Funcs::writeEntity(std::int32_t entArg) const
    {
        (m_hook) ? gpEngineFuncs->pfnWriteEntity(entArg) : WRITE_ENTITY(entArg);
    }

    void Funcs::writeAngle(float angleArg) const
    {
        (m_hook) ? gpEngineFuncs->pfnWriteAngle(angleArg) : WRITE_ANGLE(angleArg);
    }

    void Funcs::writeCoord(float coordArg) const
    {
        (m_hook) ? gpEngineFuncs->pfnWriteCoord(coordArg) : WRITE_COORD(coordArg);
    }

    void Funcs::writeString(std::string_view strArg) const
    {
        (m_hook) ? gpEngineFuncs->pfnWriteString(strArg.data()) : WRITE_STRING(strArg.data());
    }
} // namespace SPMod::Engine