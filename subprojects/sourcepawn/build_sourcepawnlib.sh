#!/bin/bash

ROOT_DIR=${1}
PROJECT_DIR=${2}

SOURCEPAWN_DEPS_DIR="$PROJECT_DIR/sourcepawn_deps"

mkdir -p "$SOURCEPAWN_DEPS_DIR"

# Install ambuild
git clone -q --depth=1 https://github.com/alliedmodders/ambuild.git "$SOURCEPAWN_DEPS_DIR/ambuild"

# Patch to build sourcepawn lib without -g3 switch (Clang only)
if [[ $CLANG_VERSION || $CC == *"clang"* || $CXX == *"clang"* ]]
then
    patch --verbose -d "$SOURCEPAWN_DEPS_DIR/ambuild/ambuild2/frontend/v2_1/cpp" < "$ROOT_DIR/patches/gcc-nodbg-clang.patch"
fi

cd "$SOURCEPAWN_DEPS_DIR/ambuild" || exit
sudo python "$SOURCEPAWN_DEPS_DIR/ambuild/setup.py" install

# Get submodules for SourcePawn lib
cd "$PROJECT_DIR/sourcepawn" || exit
git submodule update --init --recursive

# Build SourcePawn lib
mkdir -p "$SOURCEPAWN_DEPS_DIR/sourcepawn_build"
cd "$SOURCEPAWN_DEPS_DIR/sourcepawn_build" || exit
python "$PROJECT_DIR/sourcepawn/configure.py" --enable-optimize --build=core
ambuild

