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
    '-fvisibility=hidden'
  ]

  if get_option('debug') == true
    cppArgs += [
      '-g3',
      '-ggdb3',
    ]
  else
    cppArgs += [
      '-O3'
    ]
  endif

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
    '/Wall',
    '/TP',
    '/RTC1'
  ]
  linkArgs = [
    '/MACHINE:X86',
    '/SUBSYSTEM:WINDOWS',
    'kernel32.lib',
    'user32.lib',
    'gdi32.lib',
    'winspool.lib',
    'comdlg32.lib',
    'advapi32.lib',
    'shell32.lib',
    'ole32.lib',
    'oleaut32.lib',
    'uuid.lib',
    'odbc32.lib',
    'odbccp32.lib'
  ]

  if get_option('debug') == true
    cppArgs += [
      '/MTd',
      '/Od',
      '/NODEFAULTLIB:libcmt'
    ]
    linkArgs += [

    ]
  else
    cppArgs += [
      '/MT',
      '/O2',
      '/GL'
    ]
    linkArgs += [

    ]
  endif
endif

shared_library('spmod_mm', sourceFiles,
                          cpp_args : cppArgs,
                          link_args : linkArgs,
                          include_directories : includeDirs)
