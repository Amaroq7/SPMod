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

#include "ExtMain.hpp"

class ModuleInterface final : public SPMod::IInterface
{
public:
    static constexpr std::uint16_t MAJOR_VERSION = 0;
    static constexpr std::uint16_t MINOR_VERSION = 1;
    static constexpr std::uint32_t VERSION = (MAJOR_VERSION << 16 | MINOR_VERSION);

    ModuleInterface() = delete;
    ModuleInterface(const ModuleInterface &other) = delete;
    ModuleInterface(ModuleInterface &&other) = default;
    ~ModuleInterface() = default;

    ModuleInterface(fs::path &&path);

    const char *getName() const override
    {
        return "ISourcePawnModule";
    }

    std::uint32_t getVersion() const override
    {
        return VERSION;
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
        return "SourcePawn Module";
    }

    IPluginMngr *getPluginMngr() const override
    {
        return getPluginMngrCore.get();
    }

    const std::unique_ptr<PluginMngr> &getPluginMngrCore() const
    {
        return m_pluginMngr;
    }

private:
    const std::unique_ptr<PluginMngr> m_pluginMngr;
    const std::unique_ptr<SourcePawnAPI> m_sourcePawnAPI;
    const std::unique_ptr<DebugListener> m_debugListener;
};
