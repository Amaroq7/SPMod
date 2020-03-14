$SOURCEPAWN_DEPS_DIR="$env:MESON_SOURCE_ROOT\$env:MESON_SUBDIR\sourcepawn_deps"

new-item $SOURCEPAWN_DEPS_DIR -itemtype directory -Force -ErrorAction SilentlyContinue

# Install ambuild
git clone -q --depth=1 https://github.com/alliedmodders/ambuild.git $SOURCEPAWN_DEPS_DIR\ambuild

Set-Location -Path $SOURCEPAWN_DEPS_DIR\ambuild
python setup.py install

# Get submodules for SourcePawn lib
Set-Location -Path $env:MESON_SOURCE_ROOT\$env:MESON_SUBDIR\sourcepawn
git submodule update

# Build SourcePawn lib
new-item $SOURCEPAWN_DEPS_DIR\sourcepawn_build -itemtype directory -Force -ErrorAction SilentlyContinue
Set-Location -Path $SOURCEPAWN_DEPS_DIR\sourcepawn_build
python $env:MESON_SOURCE_ROOT\$env:MESON_SUBDIR\sourcepawn\configure.py --enable-optimize --build=core
ambuild

if ($env:CI -ne $null) {
    new-item $env:MESON_SOURCE_ROOT\upload\exts -itemtype directory -Force -ErrorAction SilentlyContinue
    new-item $env:MESON_SOURCE_ROOT\upload\scripting -itemtype directory -Force -ErrorAction SilentlyContinue

    Copy-Item -Path $SOURCEPAWN_DEPS_DIR\sourcepawn_build\vm\sourcepawn.jit.x86\sourcepawn.jit.x86.dll -Destination $env:MESON_SOURCE_ROOT\upload\exts
    Copy-Item -Path $SOURCEPAWN_DEPS_DIR\sourcepawn_build\compiler\spcomp\spcomp.exe -Destination $env:MESON_SOURCE_ROOT\upload\scripting
}
