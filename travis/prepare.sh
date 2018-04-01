#!/bin/bash
if [[ ! -z "${CLANG_VERSION}" ]]
then
    # Save repo dir
    REPDIR="${PWD}"

    # Overwrite default linker
    if [ ${CLANG_VERSION} == 5 ]
    then
    	export LDFLAGS=-fuse-ld=lld-5.0
    elif [ ${CLANG_VERSION} == 6 ]
    then
    	export LDFLAGS=-fuse-ld=lld-6.0
    fi

    git clone -b release_60 https://github.com/llvm-mirror/libcxx.git ../libcxx
    git clone -b release_60 https://github.com/llvm-mirror/libcxxabi.git ../libcxxabi
    mkdir ../libcxx_build ../libcxxabi_build
    cd ../libcxxabi_build
    cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS=-m32 -DLIBCXXABI_LIBCXX_INCLUDES=../libcxx/include ../libcxxabi/
    ninja
    cd ../libcxx_build
    cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DLIBCXX_CXX_ABI=libcxxabi -DLIBCXX_CXX_ABI_INCLUDE_PATHS=../libcxxabi/include -DLIBCXX_BUILD_32_BITS=ON -DCMAKE_SHARED_LINKER_FLAGS="-L../libcxxabi_build/lib" ../libcxx/
    ninja

    # Copy built files to build dir
    cd ${REPDIR}
    mkdir build
    cp ../libcxxabi_build/lib/* build
    cp ../libcxxabi/include/* include/llvm
    cp ../libcxx_build/lib/libc* build
    cp -r ../libcxx/include/* include/llvm

elif [[ ! -z "${GCC_VERSION}" ]]
then
    # Ubuntu has named dir as asm-generic while gcc searches for asm
    sudo ln -sv /usr/include/asm-generic/ /usr/include/asm

    # Overwrite default version of compilers
    # Default version is 7
    GCC_COMPILER=/usr/bin/gcc-7
    GPP_COMPILER=/usr/bin/g++-7

    sudo ln -sfv ${GCC_COMPILER} /usr/bin/gcc
    sudo ln -sfv ${GPP_COMPILER} /usr/bin/g++
fi
