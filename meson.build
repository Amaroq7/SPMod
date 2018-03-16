project('SPMod', ['c', 'cpp'],
                  license : 'GPLv3',
                  meson_version : '>=0.36.0',
                  version : '0.0.1')

add_global_arguments('-std=c11', '-m32', language : 'c')
add_global_arguments('-std=c++17', '-m32', language : 'cpp')

# TODO: build llvm stuff
find_program('ninja', required : true)
find_program('clang', required : true)
find_program('clang++', required : true)

gLinkArguments = [
  '-m32',
]

# use LLVM linker if available
if find_program('lld', required : false).found() == true
  gLinkArguments += '-fuse-ld=lld'
endif

add_global_link_arguments(gLinkArguments, language : [ 'c', 'cpp' ])

compiler = meson.get_compiler('cpp')

if compiler.get_id() != 'clang'
  error('For now only clang compiler is supported!')
endif

llvmIncludesDir = [
  join_paths('include', 'llvm')
]

spIncludesDir = [
  join_paths('include', 'sourcepawn')
]

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
                                  spIncludesDir,
                                  metamodIncludesDir,
                                  rehldsIncludesDir,
                                  llvmIncludesDir)

subdir('src')
