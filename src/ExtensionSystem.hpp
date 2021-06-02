/*
 *  Copyright (C) 2018-2020 SPMod Development Team
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

#include <IInterface.hpp>

using namespace SPMod;

class Extension final
{
public:
    enum class Status : std::uint8_t
    {
        Running = 0,
        BadLoad = 1
    };

    Extension() = delete;
    Extension(const Extension &other) = delete;
    Extension(Extension &&other) = default;

    explicit Extension(const fs::path &path);
#if defined SP_POSIX
    ~Extension() = default;
#else
    ~Extension();
#endif

    Status getStatus() const;
    void setStatus(Status status);
    fnSPModQuery getQueryFunc() const;
    fnSPModInit getInitFunc() const;
    fnSPModEnd getEndFunc() const;

private:
#if defined SP_POSIX
    std::unique_ptr<void, std::function<void(void *)>> m_extHandle;
#else
    HMODULE m_extHandle;
#endif

    fnSPModQuery m_queryFunc;
    fnSPModInit m_initFunc;
    fnSPModEnd m_endFunc;
    Status m_status;
};
