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
  cppArgs = [
    '-Wall',
    '-Wextra',
    '-Wno-unknown-pragmas',
    '-Wno-non-virtual-dtor',
    '-g3',
    '-ggdb',
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
  # support for msvc
endif

shared_library('spmod_mm', sourceFiles,
                          cpp_args : cppArgs,
                          link_args : linkArgs,
                          include_directories : includeDirs)
