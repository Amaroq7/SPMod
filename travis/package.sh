#!/bin/bash

# Set compression level
export XZ_OPT="-9e --threads=0"

# Make folders
mkdir -p "$TRAVIS_BUILD_DIR/upload/dlls"
mkdir -p "$TRAVIS_BUILD_DIR/upload/scripting"

# Move SPMod lib to dlls
cp "$TRAVIS_BUILD_DIR/build/src/libspmod_mm.so" "$TRAVIS_BUILD_DIR/upload/dlls"

# Copy sourcepawn library (fails silently if not built)
cp "$TRAVIS_BUILD_DIR/build/subprojects/sourcepawn/libsourcepawn.so" "$TRAVIS_BUILD_DIR/upload/exts" 2>/dev/null

if [[ $TRAVIS_TAG ]]
then
    ARCHIVE_NAME="spmod-$TRAVIS_OS_NAME-$TRAVIS_TAG-$CC$CC_VERSION-$LINK_TYPE.tar.xz"
else
    ARCHIVE_NAME="spmod-$TRAVIS_OS_NAME-$COMMIT_NUM-$COMMIT_SHORT_SHA-$CC$CC_VERSION-$LINK_TYPE.tar.xz"
fi

cp -r "$TRAVIS_BUILD_DIR/scripting/*" "$TRAVIS_BUILD_DIR/upload/scripting"

cd "$TRAVIS_BUILD_DIR/upload" || exit
if [[ $LINK_TYPE == "dynamic" && ! -z $CLANG_VERSION ]]
then
    mkdir -p libs
    cp "$TRAVIS_BUILD_DIR/build/libc++abi.so.1" libs
    cp "$TRAVIS_BUILD_DIR/build/libc++.so.1" libs

    tar -cJvf "$ARCHIVE_NAME" exts dlls scripting libs
else
    tar -cJvf "$ARCHIVE_NAME" exts dlls scripting
fi

mkdir -p s3
cp "$ARCHIVE_NAME" s3
