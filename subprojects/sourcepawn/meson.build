project('sourcepawn', ['c', 'cpp'],
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
            llvmIncludesDir = join_paths('..', '..', 'include', 'llvm')
        endif
    endif
endif

sourceFiles = files('CmdNatives.cpp',
                    'CoreNatives.cpp',
                    'CvarsNatives.cpp',
                    'DebugListener.cpp',
                    'ExtMain.cpp',
                    'FloatNatives.cpp',
                    'ForwardsNatives.cpp',
                    'Listeners.cpp',
                    'MenuNatives.cpp',
                    'MessageNatives.cpp',
                    'ModuleInterface.cpp',
                    'PlayerNatives.cpp',
                    'PluginSystem.cpp',
                    'PrintfImpl.cpp',
                    'SourcePawnAPI.cpp',
                    'StringNatives.cpp',
                    'TimerNatives.cpp')

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

sourcepawnIncludesDir = [
    join_paths('sourcepawn', 'include')
]

if build_machine.system() == 'linux'
    add_project_link_arguments('-ldl', language : [ 'c', 'cpp'])
endif

includeThirdPartyDirs = include_directories(publicIncludesDir,
                                            rehldsIncludesDir,
                                            metamodIncludesDir,
                                            llvmIncludesDir,
                                            sourcepawnIncludesDir,
                                            is_system: true)

shared_library('sourcepawn', sourceFiles, include_directories : includeThirdPartyDirs, gnu_symbol_visibility: 'hidden')

if build_machine.system() == 'linux'
    run_command(meson.current_source_dir() + '/build_sourcepawnlib.sh', check: true)
else
    currentPolicy = run_command('powershell', 'Get-ExecutionPolicy -Scope CurrentUser').stdout().strip()
    run_command('powershell', 'Set-ExecutionPolicy -ExecutionPolicy Bypass -Scope CurrentUser', check: true)
    run_command('powershell', '-File', meson.current_source_dir() + '\\build_sourcepawnlib.ps1', check: true)
    run_command('powershell', 'Set-ExecutionPolicy -Scope CurrentUser -ExecutionPolicy ' + currentPolicy, check: true)
endif
