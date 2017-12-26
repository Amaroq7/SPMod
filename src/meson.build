sourceFiles = files('pluginMngr.cpp',
                    'coreNatives.cpp',
                    'h_export.cpp',
                    'engine_api.cpp',
                    'meta_api.cpp',
                    'pyGlobal.cpp')

cppArgs = [
  '-stdlib=libc++',
  '-Wno-unknown-pragmas',
  '-g3',
  '-ggdb',
  '-fvisibility=hidden'
]

linkArgs = [
  '-lc++',
  '-lc++abi',
  '-L.',
  '-lpython3.7dm',
  'libc++experimental.a'
]

shared_library('pymod_mm', sourceFiles,
                          cpp_args : cppArgs,
                          link_args : linkArgs,
                          include_directories : includeDirs)
