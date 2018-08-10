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
    Extension() = delete;
    explicit Extension(fs::path path);
    ~Extension();

    void *metaHandle() const;
    void setMetaHandle(void *handle);
    fnSPModQuery getQueryFunc() const;
    fnSPModInit getInitFunc() const;
    fnSPModEnd getEndFunc() const;

private:
#if defined SP_POSIX
    void *m_extHandle;
#else
    HMODULE *m_extHandle;
#endif

    void *m_metaExtHandle;
    fnSPModQuery m_queryFunc;
    fnSPModInit m_initFunc;
    fnSPModEnd m_endFunc;
};
