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

#include "ExtMain.hpp"

namespace SPExt
{
    class AdapterInterface final : public SPMod::IAdapterInterface
    {
    public:
        static constexpr std::uint16_t MAJOR_VERSION = 0;
        static constexpr std::uint16_t MINOR_VERSION = 1;
        static constexpr std::uint32_t VERSION = (MAJOR_VERSION << 16 | MINOR_VERSION);

        AdapterInterface() = delete;
        AdapterInterface(const AdapterInterface &other) = delete;
        AdapterInterface(AdapterInterface &&other) = delete;
        ~AdapterInterface() = default;

        AdapterInterface(const fs::path &path);

        std::string_view getName() const override
        {
            return "ISourcePawnAdapter";
        }

        std::uint32_t getVersion() const override
        {
            return VERSION;
        }

        std::string_view getAuthor() const override
        {
            return "SPMod Development Team";
        }

        std::string_view getUrl() const override
        {
            return "https://github.com/Amaroq7/SPMod";
        }

        std::string_view getExtName() const override
        {
            return "SourcePawn Adapter";
        }

        PluginMngr *getPluginMngr() const override
        {
            return m_pluginMngr.get();
        }

    private:
        const std::unique_ptr<SourcePawnAPI> m_sourcePawnAPI;
        const std::unique_ptr<PluginMngr> m_pluginMngr;
        const std::unique_ptr<DebugListener> m_debugListener;
    };
} // namespace SPExt