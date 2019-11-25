$SOURCEPAWN_DEPS_DIR=$env:MESON_SOURCE_ROOT\$env:MESON_SUBDIR\sourcepawn_deps

new-item $SOURCEPAWN_DEPS_DIR -itemtype directory -Force | Out-Null

# Install ambuild
git clone -q --depth=1 https://github.com/alliedmodders/ambuild.git $SOURCEPAWN_DEPS_DIR\ambuild

Set-Location -Path $SOURCEPAWN_DEPS_DIR\ambuild
python $SOURCEPAWN_DEPS_DIR\ambuild\setup.py install

# Get submodules for SourcePawn lib
Set-Location -Path $env:MESON_SOURCE_ROOT\$env:MESON_SUBDIR\sourcepawn
git submodule update

# Build SourcePawn lib
new-item $SOURCEPAWN_DEPS_DIR\sourcepawn_build -itemtype directory -Force | Out-Null
Set-Location -Path $SOURCEPAWN_DEPS_DIR\sourcepawn_build
python $env:MESON_SOURCE_ROOT\$env:MESON_SUBDIR\sourcepawn\configure.py --enable-optimize --build=core
ambuild

if ($env:CI -ne $null) {
    new-item $env:MESON_SOURCE_ROOT\upload\dlls -itemtype directory -Force | Out-Null
    new-item $env:MESON_SOURCE_ROOT\upload\scripting -itemtype directory -Force | Out-Null

    Copy-Item -Path $SOURCEPAWN_DEPS_DIR\sourcepawn_build\vm\sourcepawn.jit.x86\sourcepawn.jit.x86.so -Destination $env:MESON_SOURCE_ROOT\upload\dlls
    Copy-Item -Path $SOURCEPAWN_DEPS_DIR\sourcepawn_build\compiler\spcomp\spcomp -Destination $env:MESON_SOURCE_ROOT\upload\scripting
}
