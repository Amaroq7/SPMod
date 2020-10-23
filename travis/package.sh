#!/bin/bash

# Set compression level
export XZ_OPT="-9e --threads=0"

if [[ $TRAVIS_TAG ]]
then
    ARCHIVE_NAME="spmod-$TRAVIS_OS_NAME-$TRAVIS_TAG-$CC$CC_VERSION-$LINK_TYPE.tar.xz"
else
    ARCHIVE_NAME="spmod-$TRAVIS_OS_NAME-$COMMIT_NUM-$COMMIT_SHORT_SHA-$CC$CC_VERSION-$LINK_TYPE.tar.xz"
fi

cd "$TRAVIS_BUILD_DIR/upload" || exit
if [[ $LINK_TYPE == "dynamic" && ! -z $CLANG_VERSION ]]
then
    tar -cJvf "$ARCHIVE_NAME" exts dlls scripting libs
else
    tar -cJvf "$ARCHIVE_NAME" exts dlls scripting
fi

mkdir -p s3
cp "$ARCHIVE_NAME" s3
