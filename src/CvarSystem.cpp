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

#include "spmod.hpp"

Cvar *CvarMngr::registerCvar(std::string_view name, std::string_view value, ICvar::Flags flags)
{
    if (Cvar *cvar = findCvar(name))
    {
        return cvar;
    }

    // Else create and register
    cvar_t new_cvar;
    new_cvar.name = name.data();
    new_cvar.string = const_cast<char *>("");
    new_cvar.flags = static_cast<int>(flags);

    CVAR_REGISTER(&new_cvar);

    // Check if really registered
    cvar_t *pcvar = CVAR_GET_POINTER(name.data());

    if (!pcvar)
        return nullptr;

    g_engfuncs.pfnCvar_DirectSet(pcvar, value.data());

    // Always add to cache
    return m_cvars.emplace(name, std::make_unique<Cvar>(name, value, flags, pcvar)).first->second.get();
}

Cvar *CvarMngr::findCvar(std::string_view name)
{
    if (Cvar *cvar = getCvar(name); cvar)
    {
        return cvar;
    }

    // Check if really registered, if so cache it
    if (cvar_t *pcvar = CVAR_GET_POINTER(name.data()); pcvar)
    {
        // Always add to cache
        return m_cvars
            .emplace(name, std::make_unique<Cvar>(name, pcvar->string, static_cast<Cvar::Flags>(pcvar->flags), pcvar))
            .first->second.get();
    }

    // Not found
    return nullptr;
}

Cvar *CvarMngr::getCvar(std::string_view name)
{
    if (auto iter = m_cvars.find(name.data()); iter != m_cvars.end())
        return iter->second.get();

    return nullptr;
}

void CvarMngr::clearCvars()
{
    m_cvars.clear();
}

void CvarMngr::clearCvarsCallback()
{
    for (const auto &pair : m_cvars)
        pair.second->clearCallback();
}

Cvar::Cvar(std::string_view name, std::string_view value, ICvar::Flags flags, cvar_t *pcvar)
    : m_flags(flags), m_name(name), m_value(value), m_cvar(pcvar)
{
}

std::string_view Cvar::getName() const
{
    return m_name;
}

Cvar::Flags Cvar::getFlags() const
{
    return m_flags;
}

void Cvar::setValue(float val)
{
    setValue(std::to_string(val));
}

void Cvar::setValue(std::int32_t val)
{
    setValue(std::to_string(val));
}

void Cvar::setFlags(Flags flags)
{
    m_flags = flags;
    m_cvar->flags = static_cast<int>(flags);
}

std::int32_t Cvar::asInt() const
{
    try
    {
        return std::stoi(m_value);
    }
    catch (const std::exception &e [[maybe_unused]])
    {
        return 0;
    }
}

float Cvar::asFloat() const
{
    try
    {
        return std::stof(m_value);
    }
    catch (const std::exception &e [[maybe_unused]])
    {
        return 0.0f;
    }
}

std::string_view Cvar::asString() const
{
    return m_value;
}

void Cvar::addCallback(Callback callback)
{
    m_callbacks.emplace_back(callback);
}

void Cvar::runCallbacks(std::string_view old_value, std::string_view new_value) const
{
    for (auto callback : m_callbacks)
    {
        callback(this, old_value, new_value);
    }
}

void Cvar::clearCallback()
{
    m_callbacks.clear();
}

void Cvar::setValue(std::string_view val)
{
    runCallbacks(m_value, val);
    m_value = val;
    g_engfuncs.pfnCvar_DirectSet(m_cvar, val.data());
}
