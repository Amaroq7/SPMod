project('SPMod', ['c', 'cpp'],
                  license : 'GPLv3',
                  meson_version : '>=0.44.0',
                  version : '0.0.1')

gLinkArguments = []

find_program('ninja', required : true)
cppCompiler = meson.get_compiler('cpp')

llvmIncludesDir = []
if build_machine.system() == 'linux'
  add_global_arguments('-m32', language : ['c', 'cpp'])
  gLinkArguments = [
    '-m32'
  ]
  if cppCompiler.get_id() == 'clang'
    if cppCompiler.version() < '5.0.0'
      warning('Clang v' + cppCompiler.version() + ' doesn\'t fully support C++17.')
    endif
    llvmIncludesDir += join_paths('include', 'llvm')
    # use LLVM linker if available
    if find_program('lld-6.0', 'lld-5.0', 'lld', required : false).found() == true
      gLinkArguments += '-fuse-ld=lld'
    endif
  elif cppCompiler.get_id() == 'gcc'
    if cppCompiler.version() < '7.0.0'
      warning('GCC v' + cppCompiler.version() + ' doesn\'t fully support C++17.')
    endif
  else
    error('Either Clang or GCC is supported.')
  endif
elif build_machine.system() == 'windows'
  gLinkArguments = '/MACHINE:X86'
  if cppCompiler.get_id() != 'msvc'
    error('Only MSVC is supported.')
  endif
  # At the time of writing MSVC support is not mature enough
  # We will setup compiler/linker flags by ourselves later
  if get_option('buildtype') != 'plain'
    warning('Configure build dir with "meson configure -Dbuildtype=plain"')
  endif
endif

add_global_link_arguments(gLinkArguments, language : [ 'c', 'cpp' ])

spIncludesDir = [
  join_paths('include', 'sourcepawn', 'include')
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
