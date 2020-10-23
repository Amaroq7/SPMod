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

#pragma once

#include "../spmod.hpp"

namespace SPMod::Engine
{
    class Engine final : public IEngine
    {
    public:
        Engine();
        Engine(const Engine &other) = delete;
        Engine(Engine &&other) = delete;
        ~Engine() = default;

        // IEngine
        Edict *getEdict(std::uint32_t index) override;
        Globals *getGlobals() const override;
        Funcs *getFuncs(bool hook) const override;
        TraceResult *createTraceResult() override;
        void freeTraceResult(ITraceResult *tr) override;

        // Engine
        Edict *getEdict(edict_t *edict);
        EntVars *getEntVars(entvars_t *vars);
        TraceResult *createTraceResult(::TraceResult *tr);
        void clear();

    private:
        std::unique_ptr<Globals> m_globals;
        std::unique_ptr<Funcs> m_funcs;
        std::unique_ptr<Funcs> m_funcsHooked;
        std::unordered_map<std::uint32_t, std::unique_ptr<Edict>> m_edicts;
        std::vector<std::unique_ptr<TraceResult>> m_traceResults;
    };
} // namespace SPMod::Engine
