/*
 *  Copyright (C) 2019 SPMod Development Team
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

#include "ExtMain.hpp"

namespace SPLuaAdapter
{
    class Interface final : public SPMod::IAdapterInterface
    {
    public:
        Interface();

        const char *getName() const override
        {
            return "ISPLuaAdapter";
        }

        uint32_t getVersion() const override
        {
            return 1U;
        }

        const char *getAuthor() const override
        {
            return "SPMod Development Team";
        }

        const char *getUrl() const override
        {
            return "https://github.com/Amaroq7/SPMod";
        }

        const char *getExtName() const override
        {
            return "SPMod Lua Adapter";
        }

        SPMod::IPluginMngr *getPluginMngr() const override
        {
            return m_pluginMngr.get();
        }

        const std::unique_ptr<PluginMngr> &getPluginMngrCore() const
        {
            return m_pluginMngr;
        }

    private:
        const std::unique_ptr<PluginMngr> m_pluginMngr;
        // const std::unique_ptr<DebugListener> m_debugListener;
    };
} // namespace SPLuaAdapter

extern SPMod::ISPGlobal *gSPGlobal;
extern SPMod::ILogger *gLogger;
extern SPLuaAdapter::Interface gInterface;