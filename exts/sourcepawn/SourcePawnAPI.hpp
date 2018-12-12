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

#include <extdll.h>
#include <ISPGlobal.hpp>

#include <memory>

#if __has_include(<filesystem>)
    #include <filesystem>
    // As of GCC 8.1 and Clang 7 filesystem is no longer part of experimental
    #if (defined SP_GCC && __GNUC__ >= 8) || (defined SP_CLANG && __clang_major__ >= 7)
        namespace fs = std::filesystem;
    #else // Some compilers still have filesystem within experimental namespace like MSVC
        namespace fs = std::experimental::filesystem;
    #endif
#elif __has_include(<experimental/filesystem>)
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
#else
    #error Filesystem header missing
#endif

#ifdef SP_CLANG
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wnon-virtual-dtor"
    #pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined SP_GCC
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
    #pragma GCC diagnostic ignored "-Wunused-parameter"
    #pragma GCC diagnostic ignored "-Wpedantic"
#elif defined SP_MSVC
    #pragma warning(push)
    // Unreferenced formal parameter
    #pragma warning(disable : 4100)
#endif
#include <sp_vm_api.h>
#ifdef SP_CLANG
    #pragma clang diagnostic pop
#elif defined SP_GCC
    #pragma GCC diagnostic pop
#elif defined SP_MSVC
    #pragma warning(pop)
#endif

namespace SPExt
{
    class SourcePawnAPI
    {
    public:
#if defined SP_LINUX
        static constexpr auto *sourcepawnLibrary = "sourcepawn.jit.x86.so";
#elif defined SP_WINDOWS
        static constexpr auto *sourcepawnLibrary = "sourcepawn.jit.x86.dll";
#endif
        SourcePawnAPI() = delete;
        SourcePawnAPI(fs::path libraryDir);
        SourcePawnAPI(const SourcePawnAPI &other) = delete;
        SourcePawnAPI(SourcePawnAPI &&other) = default;
        ~SourcePawnAPI();

        SourcePawn::ISourcePawnEnvironment *getSPEnvironment() const;

    private:
#if defined SP_POSIX
        void *m_SPLibraryHandle;
#else
        HMODULE m_SPLibraryHandle;
#endif
        SourcePawn::ISourcePawnFactory *m_spFactory;
    };

    void initializeSourcePawn();
    extern std::unique_ptr<SourcePawnAPI> gSourcePawnAPI;

    extern sp_nativeinfo_t gCoreNatives[];
    extern sp_nativeinfo_t gCvarsNatives[];
    extern sp_nativeinfo_t gForwardsNatives[];
    extern sp_nativeinfo_t gStringNatives[];
    extern sp_nativeinfo_t gMessageNatives[];
    extern sp_nativeinfo_t gCmdsNatives[];
    extern sp_nativeinfo_t gTimerNatives[];
    extern sp_nativeinfo_t gMenuNatives[];
    extern sp_nativeinfo_t gFloatNatives[];
    extern sp_nativeinfo_t gPlayerNatives[];
}