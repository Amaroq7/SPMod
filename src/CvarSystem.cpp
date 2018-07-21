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
    if (auto cvar = findCvarCore(name))
    {
        return cvar;
    }

    // Not found in cache, check if already registered
    cvar_t *pcvar = nullptr;
    std::shared_ptr<Cvar> cvar;
    pcvar = CVAR_GET_POINTER(name.data());
    
    if (pcvar)
    {
        cvar = std::make_shared<Cvar>(name, m_id, pcvar->string, static_cast<ICvar::Flags>(pcvar->flags), pcvar);
    }
    else
    {
        // Else create and register
        cvar_t new_cvar;
        new_cvar.name = name.data();
        new_cvar.string = const_cast<char*>("");
        new_cvar.flags = static_cast<int>(flags);
        
        CVAR_REGISTER(&new_cvar);

        // Check if really registered
        pcvar = CVAR_GET_POINTER(name.data());

        if (!pcvar)
            return nullptr;

        g_engfuncs.pfnCvar_DirectSet(pcvar, value.data());

        cvar = std::make_shared<Cvar>(name, m_id, value, flags, pcvar);
    }

    // Always add to cache
    m_cvars.emplace(name, cvar);
    // Raise cvar num
    m_id++;
    return cvar;
}

ICvar *CvarMngr::findCvar(const char *name)
{
    if (auto found = findCvarCore(name))
    {        
        return found.get();
    }

    cvar_t *pcvar = nullptr;
    pcvar = CVAR_GET_POINTER(name);
    if (pcvar)
    {
        std::shared_ptr<Cvar> cvar = std::make_shared<Cvar>(name, m_id, pcvar->string, static_cast<ICvar::Flags>(pcvar->flags), pcvar);
        // Always add to cache
        m_cvars.emplace(name, cvar);
        // Raise cvar num
        m_id++;
        return cvar.get();
    }
    // Not found
    return nullptr;
}

std::shared_ptr<Cvar> CvarMngr::findCvarCore(std::string_view name)
{
    auto pair = m_cvars.find(name.data());

    if (pair != m_cvars.end())
        return pair->second;

    return nullptr;
}

std::shared_ptr<Cvar> CvarMngr::findCvarCore(size_t id)
{
    for (auto pair = m_cvars.begin(); pair != m_cvars.end(); pair++)
    {
        if (pair->second->getId() == id)
            return pair->second;
    }

    return nullptr;
}
