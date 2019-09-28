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

#pragma once

#include "spmod.hpp"

class Extension final
{
public:
    enum class Status : uint8_t
    {
        Running = 0,
        BadLoad = 1
    };

    Extension() = delete;
    Extension(const Extension &other) = delete;
    Extension(Extension &&other) = default;

    explicit Extension(const fs::path &path);
    ~Extension();

    Status getStatus() const;
    void setStatus(Status status);
    fnSPModQuery getQueryFunc() const;
    fnSPModInit getInitFunc() const;
    fnSPModEnd getEndFunc() const;

private:
#if defined SP_POSIX
    void *m_extHandle;
#else
    HMODULE m_extHandle;
#endif

    fnSPModQuery m_queryFunc;
    fnSPModInit m_initFunc;
    fnSPModEnd m_endFunc;
    Status m_status;
};
