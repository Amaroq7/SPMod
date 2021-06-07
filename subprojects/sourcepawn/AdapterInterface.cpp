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

#include "AdapterInterface.hpp"
#include "SourcePawnAPI.hpp"
#include "PluginSystem.hpp"
#include "DebugListener.hpp"

#include <memory>

std::unique_ptr<SPExt::AdapterInterface> gAdapterInterface;

namespace SPExt
{
    AdapterInterface::AdapterInterface(const fs::path &path)
        : m_sourcePawnAPI(std::make_unique<SourcePawnAPI>(path)), m_pluginMngr(std::make_unique<SPExt::PluginMngr>()),
          m_debugListener(std::make_unique<DebugListener>())
    {
        m_sourcePawnAPI->getSPEnvironment()->APIv2()->SetDebugListener(m_debugListener.get());
    }

    std::string_view AdapterInterface::getName() const
    {
        return "ISourcePawnAdapter";
    }

    std::uint32_t AdapterInterface::getVersion() const
    {
        return VERSION;
    }

    std::string_view AdapterInterface::getAuthor() const
    {
        return "SPMod Development Team";
    }

    std::string_view AdapterInterface::getUrl() const
    {
        return "https://github.com/Amaroq7/SPMod";
    }

    std::string_view AdapterInterface::getExtName() const
    {
        return "SourcePawn Adapter";
    }

    SPMod::IPluginMngr *AdapterInterface::getPluginMngr() const
    {
        return m_pluginMngr.get();
    }

    PluginMngr *AdapterInterface::getPluginMngrImpl() const
    {
        return m_pluginMngr.get();
    }
} // namespace SPExt
