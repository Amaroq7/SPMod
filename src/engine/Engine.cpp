/*
 *  Copyright (C) 2020 SPMod Development Team
 *
 *  This file is part of SPMod.
 *
 *  SPMod is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SPMod is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with SPMod.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "../spmod.hpp"

namespace SPMod::Engine
{
    Engine::Engine()
        : m_globals(std::make_unique<Globals>()), m_funcs(std::make_unique<Funcs>()),
          m_funcsHooked(std::make_unique<Funcs>(true))
    {
        m_edicts.reserve(MAX_EDICTS);
    }

    Edict *Engine::getEdict(std::uint32_t index)
    {
        try
        {
            return m_edicts.at(index).get();
        }
        catch (const std::out_of_range &e [[maybe_unused]])
        {
            // check if edict is valid and register it
            if (edict_t *edict = INDEXENT(index); !FNullEnt(edict) || !index)
            {
                auto resultIter = m_edicts.emplace(index, std::make_unique<Edict>(edict)).first;
                return (*resultIter).second.get();
            }
            return nullptr;
        }
    }

    Globals *Engine::getGlobals() const
    {
        return m_globals.get();
    }

    Funcs *Engine::getFuncs(bool hook) const
    {
        return (hook) ? m_funcsHooked.get() : m_funcs.get();
    }

    TraceResult *Engine::createTraceResult()
    {
        return m_traceResults.emplace_back().get();
    }

    EntVars *Engine::getEntVars(entvars_t *vars)
    {
        try
        {
            return m_edicts.at(ENTINDEX(ENT(vars)))->getEntVars();
        }
        catch (const std::out_of_range &e [[maybe_unused]])
        {
            // check if edict is valid and register it
            if (!FNullEnt(vars) || !OFFSET(vars))
            {
                edict_t *edict = ENT(vars);
                auto resultIter = m_edicts.emplace(ENTINDEX(edict), std::make_unique<Edict>(edict)).first;
                return (*resultIter).second->getEntVars();
            }
            return nullptr;
        }
    }

    TraceResult *Engine::createTraceResult(::TraceResult *tr)
    {
        return m_traceResults.emplace_back(std::make_unique<TraceResult>(tr)).get();
    }

    void Engine::freeTraceResult(ITraceResult *tr)
    {
        for (auto iter = m_traceResults.begin(); iter != m_traceResults.end(); ++iter)
        {
            if ((*iter).get() == tr)
            {
                m_traceResults.erase(iter);
                break;
            }
        }
    }

    void Engine::clear()
    {
        m_edicts.clear();
        m_traceResults.clear();
    }

} // namespace SPMod::Engine
