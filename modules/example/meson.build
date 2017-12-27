sourceFiles = files('main.cpp')

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

shared_library('example', sourceFiles,
                          cpp_args : cppArgs,
                          link_args : linkArgs,
                          include_directories : includeDirs)
