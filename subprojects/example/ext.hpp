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

#include <ISPGlobal.hpp>
#include "DummyPluginMngr.hpp"

namespace SPExtExample
{
    class Interface final : public SPMod::IAdapterInterface
    {
        std::string_view getName() const override
        {
            return "ISPExampleExt";
        }

        uint32_t getVersion() const override
        {
            return 1U;
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
            return "SPMod Example Module";
        }

        SPMod::IPluginMngr *getPluginMngr() const override
        {
            return &SPExtExample::gPluginMngr;
        }
    };
} // namespace SPExtExample

extern SPMod::ISPGlobal *gSPGlobal;
extern SPMod::ILogger *gLogger;
extern SPExtExample::Interface gInterface;