#!/bin/bash

# Set compression level
XZ_OPT="-9e --threads=0"

# Make folders
mkdir dlls

# Install ambuild
git clone https://github.com/alliedmodders/ambuild.git ../ambuild

# Patch to build sourcepawn lib without -g3 switch (Clang only)
if [[ $CLANG_VERSION ]]
then
    patch --verbose -d ../ambuild/ambuild2/frontend/v2_1/cpp < patches/gcc-nodbg-clang.patch
fi

cd ../ambuild
sudo python setup.py install
cd $TRAVIS_BUILD_DIR

# Build SourcePawn lib
mkdir include/sourcepawn/build
cd include/sourcepawn/build
python ../configure.py --enable-optimize --build=core
ambuild
mv vm/sourcepawn.jit.x86/sourcepawn.jit.x86.so $TRAVIS_BUILD_DIR/dlls
mv compiler/spcomp/spcomp $TRAVIS_BUILD_DIR/scripts
cd $TRAVIS_BUILD_DIR

# Move SPMod lib to dlls
mv build/src/libspmod_mm.so dlls

if [[ $LINK_TYPE == "dynamic" && ! -z $CLANG_VERSION ]]
then
    mkdir libs
    mv build/libc++abi.so.1 libs
    mv build/libc++.so.1 libs
    tar -cJvf spmod-$TRAVIS_OS_NAME-$TRAVIS_TAG-$CC$CC_VERSION-$LINK_TYPE.tar.xz dlls scripts libs
else
    tar -cJvf spmod-$TRAVIS_OS_NAME-$TRAVIS_TAG-$CC$CC_VERSION-$LINK_TYPE.tar.xz dlls scripts
fi
