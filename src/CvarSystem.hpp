/*
*  Copyright (C) 2018 SPMod Development Team
*
*  This file is part of SPMod.
*
*  SPMod is free software: you can redistribute it and/or modify
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

class Plugin;

class Cvar final : public ICvar
{
public:
    Cvar(std::string_view name,
         std::string_view value,
         ICvar::Flags flags, 
         cvar_t *pcvar);

    Cvar() = delete;
    ~Cvar() = default;

    const char *getName() const override;
    Flags getFlags() const override;
    void setValue(float val) override;
    void setValue(int val) override;
    void setValue(const char *val) override;
    void setFlags(Flags flags) override;
    int asInt() const override;
    float asFloat() const override;
    const char *asString() const override;
    std::string_view asStringCore() const;
    std::string_view getNameCore() const;
    void addCallback(CvarCallback callback) override;
    void runCallbacks(std::string_view old_value,
                      std::string_view new_value);

    void clearCallback();
    void setValueCore(std::string_view val);

private:
    Flags       m_flags;
    std::string m_name;
    std::string m_value;
    cvar_t      *m_cvar;
    std::vector<CvarCallback> m_callbacks;
};


class CvarMngr final : public ICvarMngr
{
public:
    CvarMngr() = default;
    ~CvarMngr() = default;

    ICvar *registerCvar(const char *name,
                        const char *value,
                        ICvar::Flags flags) override;

    ICvar *findCvar(const char *name) override;
    std::shared_ptr<Cvar> registerCvarCore(std::string_view name,
                                           std::string_view value,
                                           ICvar::Flags flags);

    std::shared_ptr<Cvar> findCvarCore(std::string_view name,
                                       bool cacheonly);

    void clearCvars();
    void clearCvarsCallback();

private:
    std::unordered_map<std::string, std::shared_ptr<Cvar>> m_cvars;
};
