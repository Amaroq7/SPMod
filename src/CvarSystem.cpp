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

Cvar::Cvar( const char * name,
            size_t id, 
            const char *value,
            ICvar::Flags flags, 
            cvar_t* pcvar)
{
    m_name = name;
    m_id = id;
    m_value = value;
    m_flags = flags;
    m_cvar = pcvar;
}

ICvar *CvarMngr::registerCvar(  const char *name, 
                                const char *value,
                                ICvar::Flags flags)
{
    auto &loggingSystem = gSPGlobal->getLoggerCore();
    if (auto found = findCvarCore(name))
    {
        // Dont allow same cvar name
        loggingSystem->LogMessageCore("Error! Cvar: %s already registered! Use find FindCvar to get this cvar or use another name.", name);
        return nullptr;
    }
    // Not found in cache, check if already registered
    cvar_t *pcvar = nullptr;
    std::shared_ptr<Cvar> cvar;
    pcvar = CVAR_GET_POINTER(name);
    if(pcvar != nullptr)
    {
        // Dont allow same cvar name
        loggingSystem->LogMessageCore("Error! Cvar: %s already registered! Use find FindCvar to get this cvar or use another name.", name);        
        return nullptr;
    }
    else
    {
        // Else create and register
        cvar_t new_cvar;
        new_cvar.name = name;
        new_cvar.flags = (int)flags;
        new_cvar.string = (char*)value;
        CVAR_REGISTER(&new_cvar);
        // Check if really registered
        pcvar = CVAR_GET_POINTER(name);
        if (pcvar != nullptr)
        {
            cvar = std::make_shared<Cvar>(name, m_id, value, flags, pcvar);
        }
        else
        {
            // failed to register
            return nullptr;
        }
    }
    // Always add to cache
    m_cvars.emplace(name, cvar);
    // Raise cvar num
    m_id++;
    return cvar.get();
}

ICvar *CvarMngr::findCvar(const char *name)
{
    if (auto found = findCvarCore(name))
    {        
        return found;
    }

    cvar_t *pcvar = nullptr;
    pcvar = CVAR_GET_POINTER(name);
    if (pcvar != nullptr)
    {
        std::shared_ptr<Cvar> cvar = std::make_shared<Cvar>(name, m_id, pcvar->string, (ICvar::Flags)pcvar->flags, pcvar);
        // Always add to cache
        m_cvars.emplace(name, cvar);
        // Raise cvar num
        m_id++;
        return cvar.get();
    }
    // Not found
    return nullptr;
}

ICvar *CvarMngr::findCvarCore(const char *name)
{
    auto pair = m_cvars.find(name);

    if (pair != m_cvars.end())
        return (pair->second).get();

    return nullptr;
}

ICvar *CvarMngr::findCvarCore(size_t id)
{
    for (auto pair = m_cvars.begin(); pair != m_cvars.end(); pair++)
    {
        if (pair->second->getId() == id)
            return (pair->second).get();
    }

    return nullptr;
}