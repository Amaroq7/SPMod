project('SPMod', ['c', 'cpp'],
                  license : 'GPLv3',
                  meson_version : '>=0.44.0',
                  version : '0.0.1')

gLinkArgs = []
gCppArgs = []
gCArgs = []

find_program('ninja', required : true)
cppCompiler = meson.get_compiler('cpp')

llvmIncludesDir = []

if build_machine.system() == 'linux'
    gCppArgs += '-m32'
    gCArgs += '-m32'
    gLinkArgs += '-m32'

    if cppCompiler.get_id() == 'clang'
        if cppCompiler.version() < '5.0.0'
            warning('Clang v' + cppCompiler.version() + ' doesn\'t fully support C++17.')
        endif
        llvmIncludesDir += join_paths('include', 'llvm')

        # use LLVM linker if available
        llvmLinker = find_program('lld-6.0', 'lld-5.0', 'lld', required : false)
        if llvmLinker.found() == true
            gLinkArgs += '-fuse-ld=' + llvmLinker.path().split('/')[-1]
        endif
    elif cppCompiler.get_id() == 'gcc'
        if cppCompiler.version() < '7.0.0'
            warning('GCC v' + cppCompiler.version() + ' doesn\'t fully support C++17.')
        endif
    else
        error('Either Clang or GCC is supported.')
    endif

    # If standard is not set, set the minimum one
    if get_option('c_std') == 'none'
        gCArgs += '-std=c11'
    endif
    if get_option('cpp_std') == 'none'
        gCppArgs += '-std=c++17'
    endif

    #-Wall and -Wextra are set by warning_level option
    gCppArgs += [
        '-Wno-unknown-pragmas',
        '-fvisibility=hidden',
        '-Werror'
    ]

    if cppCompiler.get_id() == 'clang'
        gCppArgs += '-stdlib=libc++'
        gLinkArgs += '-L.'
        if get_option('dynamic_link') == true
            gLinkArgs += [
                '-l:libc++.so.1',
                '-l:libc++abi.so.1'
            ]
        else
            gLinkArgs += [
                '-l:libc++.a',
                '-l:libc++abi.a',
                '-pthread',
            ]
        endif
        gLinkArgs += '-lc++experimental'
    elif cppCompiler.get_id() == 'gcc'
        if get_option('dynamic_link') == false
        gLinkArgs += [
            '-static-libgcc',
            '-static-libstdc++'
        ]
        endif
        gLinkArgs += '-lstdc++fs'
    endif
elif build_machine.system() == 'windows'
    if cppCompiler.get_id() != 'msvc'
        error('Only MSVC is supported.')
    endif

    gLinkArgs += '/MACHINE:X86'

    # At the time of writing MSVC support is not mature enough
    if get_option('buildtype') != 'plain'
        warning('Configure build dir with "meson .. . --buildtype plain"')
    endif

    gCppArgs += [
        '/W4',  # Warning level 4
        '/TP',  # Treat files as c++ sources
        '/WX'   # Treat warnings as errors
    ]
    gLinkArgs += [
        '/EXPORT:GiveFnptrsToDll=_GiveFnptrsToDll@8',
        '/SECTION:.data,RW'
    ]

    # MSVC support is limited, we need to set compile options here for the time being
    gCppArgs += '/std:c++17'
    if get_option('windebug') == true
        gCppArgs += '/RTC1'  # runtime checks

        if get_option('dynamic_link') == true
            gCppArgs += '/MDd'
        endif
    else
        gCppArgs += [
            '/O2',    # optimization fast code
            '/GL'     # whole program optimization
        ]

        if get_option('dynamic_link') == true
            gCppArgs += '/MD'
        endif

        gLinkArgs += [
            '/LTCG',   # linker optimization
            '/DEBUG:NONE' # Meson adds /DEBUG by default, we need to override it
        ]
    endif
endif

add_global_arguments(gCArgs, language : 'c')
add_global_arguments(gCppArgs, language : 'cpp')
add_global_link_arguments(gLinkArgs, language : [ 'c', 'cpp' ])

spIncludesDir = [
    join_paths('include', 'sourcepawn', 'include')
]

publicIncludesDir = [
    join_paths('include', 'public')
]

metamodIncludesDir = [
    join_paths('include', 'metamod-r', 'metamod', 'src')
]

rehldsIncludesDir = [
    join_paths('include', 'rehlds', 'rehlds', 'common'),
    join_paths('include', 'rehlds', 'rehlds', 'dlls'),
    join_paths('include', 'rehlds', 'rehlds', 'engine'),
    join_paths('include', 'rehlds', 'rehlds', 'game_shared'),
    join_paths('include', 'rehlds', 'rehlds', 'pm_shared'),
    join_paths('include', 'rehlds', 'rehlds', 'public'),
    join_paths('include', 'rehlds', 'rehlds', 'public', 'rehlds')
]

includeDirs = include_directories(publicIncludesDir,
                                  spIncludesDir,
                                  metamodIncludesDir,
                                  rehldsIncludesDir,
                                  llvmIncludesDir)

subdir('src')