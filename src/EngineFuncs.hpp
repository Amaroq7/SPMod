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

#pragma once

#include "spmod.hpp"

class EngineFuncs : public IEngineFuncs
{
public:
    EngineFuncs() = default;
    ~EngineFuncs() = default;

    // IEngineFuncs
    std::string_view getArg(std::uint32_t arg) const override;
    std::string_view getArgs() const override;
    std::uint32_t getArgc() const override;
    void serverPrint(std::string_view msg) const override;
    std::uint32_t precacheModel(std::string_view model) const override;
    std::uint32_t precacheSound(std::string_view sound) const override;
    std::uint32_t precacheGeneric(std::string_view generic) const override;
    void changeLevel(std::string_view level) const override;
    void serverCommand(std::string_view cmd) const override;
    void serverExecute() const override;
    void registerSrvCommand(std::string_view cmd) const override;
    void
        messageBegin(MessageDest msgDest, std::uint32_t msgType, const float *pOrigin, IEdict *pEdict) const override;
    void messageEnd() const override;
    void writeByte(std::int32_t byteArg) const override;
    void writeChar(std::int32_t charArg) const override;
    void writeShort(std::int32_t shortArg) const override;
    void writeLong(std::int32_t longArg) const override;
    void writeEntity(std::int32_t entArg) const override;
    void writeAngle(float angleArg) const override;
    void writeCoord(float coordArg) const override;
    void writeString(std::string_view strArg) const override;
};

class EngineFuncsHooked : public IEngineFuncsHooked
{
public:
    EngineFuncsHooked() = default;
    ~EngineFuncsHooked() = default;

    // IEngineFuncsHooked
    std::string_view getArg(std::uint32_t arg) const override;
    std::string_view getArgs() const override;
    std::uint32_t getArgc() const override;
    void serverPrint(std::string_view msg) const override;
    std::uint32_t precacheModel(std::string_view model) const override;
    std::uint32_t precacheSound(std::string_view sound) const override;
    std::uint32_t precacheGeneric(std::string_view generic) const override;
    void changeLevel(std::string_view level) const override;
    void serverCommand(std::string_view cmd) const override;
    void serverExecute() const override;
    void registerSrvCommand(std::string_view cmd) const override;
    void messageBegin(MessageDest msgDest, std::uint32_t msgType, const float *pOrigin, IEdict *pEdict) const override;
    void messageEnd() const override;
    void writeByte(std::int32_t byteArg) const override;
    void writeChar(std::int32_t charArg) const override;
    void writeShort(std::int32_t shortArg) const override;
    void writeLong(std::int32_t longArg) const override;
    void writeEntity(std::int32_t entArg) const override;
    void writeAngle(float angleArg) const override;
    void writeCoord(float coordArg) const override;
    void writeString(std::string_view strArg) const override;
};

class EngineGlobals : public IEngineGlobals
{
public:
    EngineGlobals() = default;
    ~EngineGlobals() = default;

    float getTime() const override;
    std::string_view getMapName() const override;
};