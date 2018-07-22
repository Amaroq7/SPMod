#!/bin/bash

# Set compression level
export XZ_OPT="-9e --threads=0"

# Make folders
mkdir $TRAVIS_BUILD_DIR/dlls

# Move SPMod lib to dlls
mv $TRAVIS_BUILD_DIR/build/src/libspmod_mm.so $TRAVIS_BUILD_DIR/dlls

# Move SourcePawn files
mv $TRAVIS_BUILD_DIR/include/sourcepawn/build/vm/sourcepawn.jit.x86/sourcepawn.jit.x86.so $TRAVIS_BUILD_DIR/dlls
mv $TRAVIS_BUILD_DIR/include/sourcepawn/build/compiler/spcomp/spcomp $TRAVIS_BUILD_DIR/scripts

if [[ $LINK_TYPE == "dynamic" && ! -z $CLANG_VERSION ]]
then
    mkdir $TRAVIS_BUILD_DIR/libs
    mv $TRAVIS_BUILD_DIR/build/libc++abi.so.1 $TRAVIS_BUILD_DIR/libs
    mv $TRAVIS_BUILD_DIR/build/libc++.so.1 $TRAVIS_BUILD_DIR/libs
    tar -cJvf spmod-$TRAVIS_OS_NAME-$TRAVIS_TAG-$CC$CC_VERSION-$LINK_TYPE.tar.xz dlls scripts libs
else
    tar -cJvf spmod-$TRAVIS_OS_NAME-$TRAVIS_TAG-$CC$CC_VERSION-$LINK_TYPE.tar.xz dlls scripts
fi
