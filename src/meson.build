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

cppArgs = [
  '-stdlib=libc++',
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
  '-lc++',
  '-lc++abi',
  '-L.',
  'libc++experimental.a'
]

shared_library('spmod_mm', sourceFiles,
                          cpp_args : cppArgs,
                          link_args : linkArgs,
                          include_directories : includeDirs)
