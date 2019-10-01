/*
 *  Copyright (C) 2018 SPMod Development Team
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

#include "spmod.hpp"

class EngineFuncs : public IEngineFuncs
{
public:
    EngineFuncs() = default;
    ~EngineFuncs() = default;

    // IEngineFuncs
    const char *getArg(int arg) const override;
    const char *getArgs() const override;
    int getArgc() const override;
    void serverPrint(const char *msg) const override;
    int precacheModel(const char *model) const override;
    int precacheSound(const char *sound) const override;
    int precacheGeneric(const char *generic) const override;
    void changeLevel(const char* level) const override;
    void serverCommand(const char* cmd) const override;
    void serverExecute() const override;
    void registerSrvCommand(const char *cmd) const override;
};

class EngineFuncsHooked : public IEngineFuncsHooked
{
public:
    EngineFuncsHooked() = default;
    ~EngineFuncsHooked() = default;

    // IEngineFuncsHooked
    const char *getArg(int arg) const override;
    const char *getArgs() const override;
    int getArgc() const override;
    void serverPrint(const char *msg) const override;
    int precacheModel(const char *model) const override;
    int precacheSound(const char *sound) const override;
    int precacheGeneric(const char *generic) const override;
    void changeLevel(const char* level) const override;
    void serverCommand(const char* cmd) const override;
    void serverExecute() const override;
    void registerSrvCommand(const char *cmd) const override;
};

class EngineGlobals : public IEngineGlobals
{
public:
    EngineGlobals() = default;
    ~EngineGlobals() = default;
    

    float getTime() const override;
};