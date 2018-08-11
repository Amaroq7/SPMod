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

#include <ISPGlobal.hpp>

class ModuleInterface final : public SPMod::IInterface
{
    const char *getName() const override
    {
        return "ISPExtTest";
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
        return "SPMod Test Module";
    }
};

extern ModuleInterface gModuleInterface;
extern SPMod::ISPGlobal *gSPGlobal;