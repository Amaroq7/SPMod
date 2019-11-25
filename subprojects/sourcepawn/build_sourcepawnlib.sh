#!/bin/bash

SOURCEPAWN_DEPS_DIR=$MESON_SOURCE_ROOT/$MESON_SUBDIR/sourcepawn_deps

mkdir -p $SOURCEPAWN_DEPS_DIR

# Install ambuild
git clone -q --depth=1 https://github.com/alliedmodders/ambuild.git $SOURCEPAWN_DEPS_DIR/ambuild

# Patch to build sourcepawn lib without -g3 switch (Clang only)
if [[ $CLANG_VERSION ]]
then
    patch --verbose -d $SOURCEPAWN_DEPS_DIR/ambuild/ambuild2/frontend/v2_1/cpp < $MESON_SOURCE_ROOT/patches/gcc-nodbg-clang.patch
fi

cd $SOURCEPAWN_DEPS_DIR/ambuild
sudo python $SOURCEPAWN_DEPS_DIR/ambuild/setup.py install

# Get submodules for SourcePawn lib
cd $MESON_SOURCE_ROOT/$MESON_SUBDIR/sourcepawn
git submodule update --init --recursive

# Build SourcePawn lib
mkdir $SOURCEPAWN_DEPS_DIR/sourcepawn_build
cd $SOURCEPAWN_DEPS_DIR/sourcepawn_build
python $MESON_SOURCE_ROOT/$MESON_SUBDIR/sourcepawn/configure.py --enable-optimize --build=core
ambuild

if [[ $CI ]]
then
    mkdir -p $MESON_SOURCE_ROOT/upload/dlls
    mkdir -p $MESON_SOURCE_ROOT/upload/scripting

    cp $SOURCEPAWN_DEPS_DIR/sourcepawn_build/vm/sourcepawn.jit.x86/sourcepawn.jit.x86.so $MESON_SOURCE_ROOT/upload/dlls
    cp $SOURCEPAWN_DEPS_DIR/sourcepawn_build/compiler/spcomp/spcomp $MESON_SOURCE_ROOT/upload/scripting
fi
