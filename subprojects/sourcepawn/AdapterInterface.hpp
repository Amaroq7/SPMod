/*
 *  Copyright (C) 2020-2021 SPMod Development Team
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

#include <ISPGlobal.hpp>

namespace SPExt
{
    class PluginMngr;
    class SourcePawnAPI;
    class DebugListener;

    class AdapterInterface final : public SPMod::IAdapterInterface
    {
    public:
        static constexpr std::uint16_t MAJOR_VERSION = 0;
        static constexpr std::uint16_t MINOR_VERSION = 1;
        static constexpr std::uint32_t VERSION = (MAJOR_VERSION << 16 | MINOR_VERSION);

        AdapterInterface() = delete;
        AdapterInterface(const AdapterInterface &other) = delete;
        AdapterInterface(AdapterInterface &&other) = delete;
        ~AdapterInterface() final = default;

        explicit AdapterInterface(const fs::path &path);

        [[nodiscard]] std::string_view getName() const final;
        [[nodiscard]] std::uint32_t getVersion() const final;
        [[nodiscard]] std::string_view getAuthor() const final;
        [[nodiscard]] std::string_view getUrl() const final;
        [[nodiscard]] std::string_view getExtName() const final;
        [[nodiscard]] SPMod::IPluginMngr *getPluginMngr() const final;

        [[nodiscard]] PluginMngr *getPluginMngrImpl() const;

    private:
        const std::unique_ptr<SourcePawnAPI> m_sourcePawnAPI;
        const std::unique_ptr<PluginMngr> m_pluginMngr;
        const std::unique_ptr<DebugListener> m_debugListener;
    };
} // namespace SPExt

extern std::unique_ptr<SPExt::AdapterInterface> gAdapterInterface;