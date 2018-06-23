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
    Cvar( const char *name, size_t m_id,
          const char *value,
          Flags flags,
          cvar_t *pcvar);

    Cvar() = delete;
    ~Cvar() = default;

    const char* getName() const override
    {
        return m_name.c_str();
    }

    Flags getFlags() const override
    {
        return m_flags;
    }

    size_t getId() const override
    {
        return m_id;
    }

    void setValue(float val) override
    {
        std::string newval(std::to_string(val));
        runCallbacks(m_value.c_str(), newval.c_str());
        m_value.assign(newval);
        g_engfuncs.pfnCvar_DirectSet(m_cvar, newval.c_str());
    }

    void setValue(int val) override
    {
        std::string newval(std::to_string(val));
        runCallbacks(m_value.c_str(), newval.c_str());
        m_value.assign(newval);
        g_engfuncs.pfnCvar_DirectSet(m_cvar, newval.c_str());
    }

    void setValue(const char *val) override
    {
        std::string newval(val);
        runCallbacks(m_value.c_str(), newval.c_str());
        m_value.assign(newval);
        g_engfuncs.pfnCvar_DirectSet(m_cvar, newval.c_str());
    }

    void setFlags( Flags flags) override
    {
        m_flags = flags;
        m_cvar->flags = (int)flags;
    }
    
    int asInt() const override
    {
        return std::stol(m_value.c_str(), nullptr, 0);
    }

    float asFloat() const override
    {
        return std::stof(m_value.c_str(), nullptr);
    }
    
    const char *asString() const override
    {
        return m_value.c_str();
    }

    void addCallback(cvarCallback_t callback) override
    {
        m_callbacks.push_back(callback);
    }

    void addPluginCallback(SourcePawn::IPluginFunction *callback) override
    {
        m_plugin_callbacks.push_back(callback);
    }

    void runCallbacks(  const char *old_value,
                        const char *new_value)
    {
        for (auto callback : m_callbacks)
        {
            (*callback)(this, old_value, new_value);
        }
        for (auto callback : m_plugin_callbacks)
        {            
            callback->PushCell((cell_t)m_id);
            callback->PushString(old_value);
            callback->PushString(new_value);
            callback->Execute(NULL);
        }
    }
protected:
    Flags       m_flags;
    std::string m_name;
    std::string m_value;
    size_t m_id;
    cvar_t      *m_cvar;
    std::vector<cvarCallback_t> m_callbacks;
    std::vector<SourcePawn::IPluginFunction*> m_plugin_callbacks;
};


class CvarMngr final : public ICvarMngr
{
public:
    CvarMngr() = default;
    ~CvarMngr() = default;

    ICvar *registerCvar(const char *name, const char *value, ICvar::Flags flags) override;
    ICvar *findCvar(const char *name) override;
    ICvar *findCvarCore(const char *name );
    ICvar *findCvarCore(size_t id);
    
private:
    std::unordered_map<std::string, std::shared_ptr<Cvar>> m_cvars;
    /* keeps track of cvar ids */
    size_t m_id = 0;
};
