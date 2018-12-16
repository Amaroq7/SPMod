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

#include "CvarSystem.hpp"

ICvar *CvarMngr::registerCvar(const char *name, 
                              const char *value,
                              ICvar::Flags flags)
{
    return registerCvarCore(name, value, flags).get();
}

std::shared_ptr<Cvar> CvarMngr::registerCvarCore(std::string_view name,
                                                 std::string_view value,
                                                 ICvar::Flags flags)
{
    if (auto cvar = findCvarCore(name, true))
    {
        return cvar;
    }

    // Not found in cache, check if already registered
    cvar_t *pcvar = nullptr;
    std::shared_ptr<Cvar> cvar;
    pcvar = CVAR_GET_POINTER(name.data());
    
    if (pcvar)
    {
        cvar = std::make_shared<Cvar>(name, pcvar->string, static_cast<ICvar::Flags>(pcvar->flags), pcvar);
    }
    else
    {
        // Else create and register
        cvar_t new_cvar;
        new_cvar.name = name.data();
        new_cvar.string = const_cast<char *>("");
        new_cvar.flags = static_cast<int>(flags);

        CVAR_REGISTER(&new_cvar);

        // Check if really registered
        pcvar = CVAR_GET_POINTER(name.data());

        if (!pcvar)
            return nullptr;

        g_engfuncs.pfnCvar_DirectSet(pcvar, value.data());

        cvar = std::make_shared<Cvar>(name, value, flags, pcvar);
    }

    // Always add to cache
    m_cvars.emplace(name, cvar);

    return cvar;
}

ICvar *CvarMngr::findCvar(const char *name)
{
    return findCvarCore(name, false).get();
}

std::shared_ptr<Cvar> CvarMngr::findCvarCore(std::string_view name,
                                             bool cacheonly)
{
    auto pair = m_cvars.find(name.data());
    if (pair != m_cvars.end())
        return pair->second;

    if (cacheonly)
        return nullptr;

    cvar_t *pcvar = nullptr;
    pcvar = CVAR_GET_POINTER(name.data());
    if (pcvar)
    {
        std::shared_ptr<Cvar> cvar = std::make_shared<Cvar>(name, pcvar->string, static_cast<ICvar::Flags>(pcvar->flags), pcvar);
        // Always add to cache
        m_cvars.emplace(name, cvar);

        return cvar;
    }
    // Not found
    return nullptr;
}

void CvarMngr::clearCvars()
{
    m_cvars.clear();
}

void CvarMngr::clearCvarsCallback()
{
    for (auto pair : m_cvars)
        pair.second->clearCallback();
}

Cvar::Cvar(std::string_view name,
           std::string_view value,
           ICvar::Flags flags, 
           cvar_t *pcvar) : m_flags(flags),
                            m_name(name),
                            m_value(value),
                            m_cvar(pcvar)
{ 
}

const char *Cvar::getName() const
{
    return m_name.c_str();
}

Cvar::Flags Cvar::getFlags() const
{
    return m_flags;
}

void Cvar::setValue(float val)
{
    setValueCore(std::to_string(val));
}

void Cvar::setValue(int val)
{
    setValueCore(std::to_string(val));
}

void Cvar::setValue(const char *val)
{
    setValueCore(val);
}

void Cvar::setFlags(Flags flags)
{
    m_flags = flags;
    m_cvar->flags = static_cast<int>(flags);
}

int Cvar::asInt() const
{
    try
    {
        return std::stoi(m_value, nullptr, 0);
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

const char *Cvar::asString() const
{
    return m_value.c_str();
}

std::string_view Cvar::asStringCore() const
{
    return m_value;
}

std::string_view Cvar::getNameCore() const
{
    return m_name;
}

void Cvar::addCallback(CvarCallback callback)
{
    m_callbacks.push_back(callback);
}

void Cvar::runCallbacks(std::string_view old_value,
                        std::string_view new_value)

{
    for (auto callback : m_callbacks)
    {
        callback(this, old_value.data(), new_value.data());
    }
}

void Cvar::clearCallback()
{
    m_callbacks.clear();
}

void Cvar::setValueCore(std::string_view val)
{
    runCallbacks(m_value, val);
    m_value.assign(val);
    g_engfuncs.pfnCvar_DirectSet(m_cvar, val.data());
}
