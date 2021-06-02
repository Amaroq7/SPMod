/*
 *  Copyright (C) 2020 SPMod Development Team

 *  This file is part of SPMod.

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

#include <SPConfig.hpp>
#include <public/IMetamod.hpp>
#include <public/game/ILibrary.hpp>
#include <public/engine/ILibrary.hpp>

class MetaPlugin final : public Metamod::IPlugin
{
public:
    ~MetaPlugin() final = default;

    std::uint32_t getInterfaceVersion() const final
    {
        return Metamod::IMetamod::VERSION;
    }

    std::string_view getName() const final
    {
        return "SPMod";
    }

    std::string_view getVersion() const final
    {
        return gSPModVersion;
    }

    std::string_view getDate() const final
    {
        return __DATE__;
    }

    std::string_view getAuthor() const final
    {
        return gSPModAuthor;
    }

    std::string_view getUrl() const final
    {
        return "https://github.com/Amaroq7/SPMod";
    }

    std::string_view getLogTag() const final
    {
        return gSPModLoggerName;
    }
};

extern MetaPlugin metaPlugin;
extern Metamod::IMetamod *gMetaAPI;
extern Metamod::Engine::ILibrary *gEngine;
extern Metamod::Game::ILibrary *gGame;