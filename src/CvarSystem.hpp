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
               size_t id, 
               std::string_view value,
               ICvar::Flags flags, 
               cvar_t *pcvar) : m_flags(flags),
                                m_name(name),
                                m_value(value),
                                m_id(id),
                                m_cvar(pcvar)
    {}

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
        runCallbacks(m_value, newval);
        m_value.assign(newval);
        g_engfuncs.pfnCvar_DirectSet(m_cvar, newval.c_str());
    }

    void setValue(int val) override
    {
        std::string newval(std::to_string(val));
        runCallbacks(m_value, newval);
        m_value.assign(newval);
        g_engfuncs.pfnCvar_DirectSet(m_cvar, newval.c_str());
    }

    void setValue(const char *val) override
    {
        std::string newval(val);
        runCallbacks(m_value, newval);
        m_value.assign(newval);
        g_engfuncs.pfnCvar_DirectSet(m_cvar, newval.c_str());
    }

    void setFlags(Flags flags) override
    {
        m_flags = flags;
        m_cvar->flags = static_cast<int>(flags);
    }
    
    int asInt() const override
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

    float asFloat() const override
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
    
    const char *asString() const override
    {
        return m_value.c_str();
    }

    std::string_view asStringCore() const
    {
        return m_value;
    }

    void addCallback(cvarCallback_t callback) override
    {
        m_callbacks.push_back(callback);
    }

    void addPluginCallback(SourcePawn::IPluginFunction *callback) override
    {
        m_plugin_callbacks.push_back(callback);
    }

    void runCallbacks(std::string_view old_value,
                      std::string_view new_value)

    {
        for (auto callback : m_callbacks)
        {
            callback(this, old_value.data(), new_value.data());
        }
        for (auto callback : m_plugin_callbacks)
        {            
            callback->PushCell(static_cast<cell_t>(m_id));
            callback->PushString(old_value.data());
            callback->PushString(new_value.data());
            callback->Execute(nullptr);
        }
    }

    void clearCallback()
    {
        m_callbacks.clear();
        m_plugin_callbacks.clear();
    }
private:
    Flags       m_flags;
    std::string m_name;
    std::string m_value;
    size_t      m_id;
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
    std::shared_ptr<Cvar> registerCvarCore(std::string_view name, std::string_view value, ICvar::Flags flags);
    std::shared_ptr<Cvar> findCvarCore(std::string_view name, bool cacheonly);
    std::shared_ptr<Cvar> findCvarCore(size_t id);

    void clearCvars()
    {
        m_cvars.clear();
        m_id = 0;
    }
    void clearCvarsCallback()
    {
        for (auto pair : m_cvars)
            pair.second->clearCallback();
    }
private:
    std::unordered_map<std::string, std::shared_ptr<Cvar>> m_cvars;
    /* keeps track of cvar ids */
    size_t m_id = 0;
};
