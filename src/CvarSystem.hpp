#pragma once

#include "spmod.hpp"

class Plugin;

class Cvar : public ICvar
{
public:
    Cvar(std::string_view name, size_t m_id,
            std::string_view value,
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

    void setValue(std::string_view val) override
    {
        std::string newval(val);
        runCallbacks(m_value, newval);
        m_value.assign(newval);
        g_engfuncs.pfnCvar_DirectSet(m_cvar, newval.c_str());
    }

    void setFlags( Flags flags) override
    {
        m_flags = flags;
    }

    void lockCvar()
    {
        m_lock = true;
    }

    void unlockCvar()
    {
        m_lock = false;
    }

    int asInt() const override
    {
        return strtol(m_value.c_str(), nullptr, 0);
    }

    float asFloat() const override
    {
        return strtof(m_value.c_str(), nullptr);
    }
    
    std::string asString() const override
    {
        return m_value;
    }

    void addCallback(cvarCallback_t *callback) override
    {
        m_callbacks.push_back(callback);
    }

    void addPluginCallback(SourcePawn::IPluginFunction *callback) override
    {
        m_plugin_callbacks.push_back(callback);
    }

    void runCallbacks(  std::string old_value, 
                        std::string new_value)
    {
        for (auto callback : m_callbacks)
        {
            callback(this, old_value, new_value);
        }
        for (auto callback : m_plugin_callbacks)
        {            
            callback->PushCell((cell_t)m_id);
            callback->PushString(old_value.c_str());
            callback->PushString(new_value.c_str());
            callback->Execute(NULL);
        }
    }
protected:
    Flags       m_flags;
    std::string m_name;
    std::string m_value;
    size_t m_id;
    cvar_t      *m_cvar;
    bool        m_lock = false;
    std::vector<cvarCallback_t*> m_callbacks;
    std::vector<SourcePawn::IPluginFunction*> m_plugin_callbacks;
};


class CvarMngr final : public ICvarMngr
{
public:
    CvarMngr() = default;
    ~CvarMngr() = default;

    ICvar *registerOrFindCvar(const char *name, char* value, ICvar::Flags flags, bool force_register) override;
    ICvar *findCvar(const char *name ) override;
    ICvar *findCvar(size_t id) override;
    bool setCvarCallback(ICvar *cvar, ICvar::cvarCallback_t* func) override;
    bool setCvarCallback(ICvar *cvar, SourcePawn::IPluginFunction* callback) override;
    
private:
    std::unordered_map<std::string, std::shared_ptr<Cvar>> m_cvars;
    /* keeps track of cvar ids */
    size_t m_id = 0;
};
