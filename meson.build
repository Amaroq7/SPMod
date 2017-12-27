project('PyMod', ['c', 'cpp'],
                  license : 'GPLv3',
                  meson_version : '>=0.36.0',
                  version : '1.0.0')

add_global_arguments('-std=c11', '-m32', language : 'c')
add_global_arguments('-std=c++17', '-m32', language : 'cpp')
add_global_link_arguments('-m32', language : [ 'c', 'cpp' ])

# TODO: build python & llvm stuff
find_program('ninja', required : true)
find_program('clang', required : true)
find_program('clang++', required : true)

compiler = meson.get_compiler('cpp')

if compiler.get_id() != 'clang'
  error('For now only clang compiler is supported!')
endif

llvmIncludesDir = [
  join_paths('include', 'llvm')
]

pythonIncludesDir = [
  join_paths('include', 'python')
]

publicIncludesDir = [
  join_paths('include', 'public')
]

metamodIncludesDir = [
  join_paths('include', 'metamod')
]

rehldsIncludesDir = [
  join_paths('include', 'rehlds', 'common'),
  join_paths('include', 'rehlds', 'dlls'),
  join_paths('include', 'rehlds', 'engine'),
  join_paths('include', 'rehlds', 'game_shared'),
  join_paths('include', 'rehlds', 'pm_shared'),
  join_paths('include', 'rehlds', 'public'),
  join_paths('include', 'rehlds', 'public', 'rehlds')
]

includeDirs = include_directories(publicIncludesDir,
                                  pythonIncludesDir,
                                  metamodIncludesDir,
                                  rehldsIncludesDir,
                                  llvmIncludesDir)

subdir('src')
subdir('modules')
