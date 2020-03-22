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

class Cvar final : public ICvar
{
public:
    Cvar(std::string_view name, std::string_view value, ICvar::Flags flags, cvar_t *pcvar);

    Cvar() = delete;
    ~Cvar() = default;

    std::string_view getName() const override;
    Flags getFlags() const override;
    void setValue(float val) override;
    void setValue(std::int32_t val) override;
    void setValue(std::string_view val) override;
    void setFlags(Flags flags) override;
    std::int32_t asInt() const override;
    float asFloat() const override;
    std::string_view asString() const override;
    void addCallback(Callback callback) override;
    void runCallbacks(std::string_view old_value, std::string_view new_value) const;

    void clearCallback();

private:
    Flags m_flags;
    std::string m_name;
    std::string m_value;
    cvar_t *m_cvar;
    std::vector<Callback> m_callbacks;
};

class CvarMngr final : public ICvarMngr
{
public:
    CvarMngr() = default;
    ~CvarMngr() = default;

    Cvar *registerCvar(std::string_view name, std::string_view value, ICvar::Flags flags);
    Cvar *findCvar(std::string_view name);

    Cvar *getCvar(std::string_view name);

    void clearCvars();
    void clearCvarsCallback();

private:
    std::unordered_map<std::string, std::unique_ptr<Cvar>> m_cvars;
};
