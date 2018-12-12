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

#include "SourcePawnAPI.hpp"

#include <dlfcn.h>

namespace SPExt
{
    std::unique_ptr<SourcePawnAPI> gSourcePawnAPI;

    SourcePawnAPI::SourcePawnAPI(fs::path libraryDir)
    {
        libraryDir /= SourcePawnAPI::sourcepawnLibrary;

#ifdef SP_POSIX
        void *libraryHandle = dlopen(libraryDir.c_str(), RTLD_NOW | RTLD_LOCAL | RTLD_DEEPBIND);
#else
        HMODULE libraryHandle = LoadLibrary(libraryDir.string().c_str());
#endif

        if (!libraryHandle)
            throw std::runtime_error("Failed to open SourcePawn library");

#ifdef SP_POSIX
        auto getFactoryFunc = reinterpret_cast<SourcePawn::GetSourcePawnFactoryFn>
                                    (dlsym(libraryHandle, "GetSourcePawnFactory"));
#else
        auto getFactoryFunc = reinterpret_cast<SourcePawn::GetSourcePawnFactoryFn>
                                    (GetProcAddress(libraryHandle, "GetSourcePawnFactory"));
#endif

        if (!getFactoryFunc)
        {
#ifdef SP_POSIX
            dlclose(libraryHandle);
#else
            FreeLibrary(libraryHandle);
#endif
            throw std::runtime_error("Cannot find SourcePawn factory function");
        }

        SourcePawn::ISourcePawnFactory *SPFactory = getFactoryFunc(SOURCEPAWN_API_VERSION);
        if (!SPFactory)
        {
#ifdef SP_POSIX
            dlclose(libraryHandle);
#else
            FreeLibrary(libraryHandle);
#endif
            throw std::runtime_error("Wrong SourcePawn library version");
        }

        m_SPLibraryHandle = libraryHandle;
        m_spFactory = SPFactory;
        m_spFactory->NewEnvironment();
        getSPEnvironment()->APIv2()->SetJitEnabled(true);

        // TODO: Logging
        //getSPEnvironment()->APIv2()->SetDebugListener(m_loggingSystem.get());
    }

    SourcePawnAPI::~SourcePawnAPI()
    {
#if defined SP_POSIX
            dlclose(m_SPLibraryHandle);
#else
            FreeLibrary(m_SPLibraryHandle);
#endif
    }

    SourcePawn::ISourcePawnEnvironment *SourcePawnAPI::getSPEnvironment() const
    {
        return m_spFactory->CurrentEnvironment();
    }
}