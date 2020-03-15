project('SPMod', ['c', 'cpp'],
                  license : 'GPLv3',
                  meson_version : '>=0.49.0',
                  version : '0.2.0-alpha')

gLinkArgs = []
gCppArgs = []
gCArgs = []

find_program('ninja', required : true)
git = find_program('git', required : false)
cppCompiler = meson.get_compiler('cpp')

llvmIncludesDir = ''

gExtsToBuild = get_option('extensions')

if build_machine.system() == 'linux'
    gCppArgs += '-m32'
    gCArgs += '-m32'
    gLinkArgs += '-m32'

    if cppCompiler.get_id() == 'clang'
        minVersion = '6.0.0'

        if cppCompiler.version() < minVersion
            warning('Clang v' + cppCompiler.version() + ' is not supported. Minimum version is ' + minVersion)
        endif

        libcxxLibraryHeaderPaths = [
                                    '-I/usr/lib/llvm-' + cppCompiler.version()[0] + '/include/c++/v1',
                                    '-I/usr/include/c++/v1'
                                   ]

        # Check if include files already exist
        if cppCompiler.has_header('__cxxabi_config.h', args : libcxxLibraryHeaderPaths) == false
            llvmIncludesDir = join_paths('include', 'llvm')
        endif

        # use LLVM linker if available
        llvmLinker = find_program('lld-8', 'lld-7', 'lld', required : false)
        if llvmLinker.found() == true
            gLinkArgs += '-fuse-ld=' + llvmLinker.path().split('/')[-1]
        endif

        gCppArgs += '-stdlib=libc++'
        gLinkArgs += '-L.'
        if get_option('linktype') == 'dynamic'
            gLinkArgs += [
                '-l:libc++.so.1',
                '-l:libc++abi.so.1'
            ]
        else
            gLinkArgs += [
                '-l:libc++.a',
                '-l:libc++abi.a',
                '-pthread'
            ]
        endif

        if (cppCompiler.version() < '9.0.0')
            if (cppCompiler.version() >= '7.0.0')
                gLinkArgs += '-lc++fs'
            else
                gLinkArgs += '-lc++experimental'
            endif
        endif

    elif cppCompiler.get_id() == 'gcc'
        minVersion = '7.0.0'

        if cppCompiler.version() < minVersion
            warning('GCC v' + cppCompiler.version() + ' is not supported. Minimum version is ' + minVersion)
        endif

        if get_option('linktype') == 'static'
            gLinkArgs += [
                '-static-libgcc',
                '-static-libstdc++'
            ]
        endif

        if cppCompiler.version() < '9.0.0'
            gLinkArgs += '-lstdc++fs'
        endif

    else
        error('Either Clang or GCC is supported.')
    endif
elif build_machine.system() == 'windows'
    if cppCompiler.get_id() != 'msvc'
        error('Only MSVC is supported.')
    endif

    gLinkArgs += '/MACHINE:X86'

    gCppArgs += [
        '/TP'  # Treat files as c++ sources
    ]
endif

# Generate SPConfig.hpp
confData = configuration_data()
confData.set('version', meson.project_version())

if git.found() == true
    commitSha = run_command('git', 'rev-parse', '--short', 'HEAD').stdout().strip()
else
    commitSha = '<unknown>'
endif

confData.set('commit', commitSha)
configure_file(input : 'src/SPConfig.hpp.in', output : 'SPConfig.hpp', configuration : confData)

add_global_arguments(gCArgs, language : 'c')
add_global_arguments(gCppArgs, language : 'cpp')
add_global_link_arguments(gLinkArgs, language : [ 'c', 'cpp' ])

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
                                  rehldsIncludesDir,
                                  metamodIncludesDir,
                                  llvmIncludesDir,
                                  is_system: true)

# clang workaround, remove when no longer needed
if cppCompiler.get_id() == 'clang'
    includeDirs = include_directories(publicIncludesDir,
                                      rehldsIncludesDir,
                                      metamodIncludesDir,
                                      llvmIncludesDir,
                                      '.',
                                      is_system: true)
else
    includeDirs = include_directories(publicIncludesDir,
                                      rehldsIncludesDir,
                                      metamodIncludesDir,
                                      llvmIncludesDir,
                                      is_system: true)
endif

subdir('src')

if gExtsToBuild.length() != 0
    if 'all' in gExtsToBuild
        subproject('example')
        subproject('sourcepawn')
    else
        if 'sourcepawn' in gExtsToBuild
            subproject('sourcepawn')
        endif
        if 'example' in gExtsToBuild
            subproject('example')
        endif
    endif
endif
