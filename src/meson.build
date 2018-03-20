add_project_arguments('-DSPMOD_CORE', language : [ 'c', 'cpp' ])

sourceFiles = files('coreNatives.cpp',
                    'h_export.cpp',
                    'engine_api.cpp',
                    'dllapi.cpp',
                    'meta_api.cpp',
                    'SPGlobal.cpp',
                    'rehlds_init.cpp',
                    'ForwardSystem.cpp',
                    'PluginSystem.cpp',
                    'LoggingSystem.cpp',
                    'SrvCommand.cpp')

cppArgs = []
linkArgs = []
if build_machine.system() == 'linux'
  #-Wall and -Wextra are set by warning_level option
  cppArgs = [
    '-Wno-unknown-pragmas',
    '-fvisibility=hidden'
  ]

  linkArgs = [
    '-ldl',
  ]

  if cppCompiler.get_id() == 'clang'
    cppArgs += [
      '-stdlib=libc++',
    ]
    linkArgs += [
      '-l:libc++.so.1',
      '-l:libc++abi.so.1',
      '-L.',
      'libc++experimental.a'
    ]
  elif cppCompiler.get_id() == 'gcc'
    # link libstdc++ and libgcc_s statically
    linkArgs += [
      '-static-libgcc',
      '-static-libstdc++',
      '-lstdc++fs'
    ]
  endif
elif build_machine.system() == 'windows'
  cppArgs = [
    '/W4',  # Warning level 4
    '/TP'   # Treat files as c++ sources
  ]
  linkArgs = [
    '/EXPORT:GiveFnptrsToDll=_GiveFnptrsToDll@8',
    '/SECTION:.data,RW'
  ]

  # MSVC support is limited, we need to set compile options here for the time being
  add_project_arguments('/std:c++17', language : 'cpp')
  if get_option('windebug') == true
    cppArgs += [
      '/RTC1'  # runtime checks
    ]
  else
    cppArgs += [
      '/O2',    # optimization fast code
      '/GL'     # whole program optimization
    ]
    linkArgs += [
      '/LTCG',   # linker optimization
      '/DEBUG:NONE' # Meson adds /DEBUG by default, we need to override it
    ]
  endif
endif

shared_library('spmod_mm', sourceFiles,
                          cpp_args : cppArgs,
                          link_args : linkArgs,
                          include_directories : includeDirs)
