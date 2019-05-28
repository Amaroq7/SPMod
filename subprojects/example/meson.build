project('example', ['c', 'cpp'],
                  license : 'GPLv3',
                  meson_version : '>=0.49.0',
                  version : '0.2.0-dev')

cppCompiler = meson.get_compiler('cpp')
llvmIncludesDir = ''

if build_machine.system() == 'linux'
    if cppCompiler.get_id() == 'clang'
        libcxxLibraryHeaderPaths = [
                                    '-I/usr/lib/llvm-' + cppCompiler.version()[0] + '/include/c++/v1',
                                    '-I/usr/include/c++/v1'
                                   ]

        # Check if include files already exist
        if cppCompiler.has_header('__cxxabi_config.h', args : libcxxLibraryHeaderPaths) == false
            llvmIncludesDir = join_paths('include', 'llvm')
        endif
    endif
endif

sourceFiles = files('DummyPluginMngr.cpp',
                    'ext.cpp')

publicIncludesDir = [
    join_paths('..', '..', 'include', 'public')
]

metamodIncludesDir = [
    join_paths('..', '..', 'include', 'metamod-r', 'metamod', 'src')
]

rehldsIncludesDir = [
    join_paths('..', '..', 'include', 'rehlds', 'rehlds', 'common'),
    join_paths('..', '..', 'include', 'rehlds', 'rehlds', 'dlls'),
    join_paths('..', '..', 'include', 'rehlds', 'rehlds', 'engine'),
    join_paths('..', '..', 'include', 'rehlds', 'rehlds', 'game_shared'),
    join_paths('..', '..', 'include', 'rehlds', 'rehlds', 'pm_shared'),
    join_paths('..', '..', 'include', 'rehlds', 'rehlds', 'public'),
    join_paths('..', '..', 'include', 'rehlds', 'rehlds', 'public', 'rehlds')
]

includeDirs = include_directories(publicIncludesDir,
                                  rehldsIncludesDir,
                                  metamodIncludesDir,
                                  llvmIncludesDir,
                                  '.') #build directory

shared_library('example', sourceFiles, include_directories : includeDirs)
