#!/bin/bash

# Save repo dir
REPDIR="${PWD}"

if [[ ! -z "${CLANG_VERSION}" ]]
then
    # Overwrite default compiler
    if [ ${CLANG_VERSION} == 5 ]
    then
        export LDFLAGS=-fuse-ld=lld-5.0
        BRANCH_NAME=release_50

        CLANG_COMPILER=/usr/bin/clang-5.0
        CLANGPP_COMPILER=/usr/bin/clang++-5.0
    elif [ ${CLANG_VERSION} == 6 ]
    then
        export LDFLAGS=-fuse-ld=lld-6.0
        BRANCH_NAME=release_60

        CLANG_COMPILER=/usr/bin/clang-6.0
        CLANGPP_COMPILER=/usr/bin/clang++-6.0
    fi

    sudo update-alternatives --install /usr/bin/clang clang ${CLANG_COMPILER} 1000
    sudo update-alternatives --install /usr/bin/clang++ clang++ ${CLANGPP_COMPILER} 1000

    # Build libcxxabi & libcxx
    git clone -b ${BRANCH_NAME} https://github.com/llvm-mirror/libcxx.git ../libcxx
    git clone -b ${BRANCH_NAME} https://github.com/llvm-mirror/libcxxabi.git ../libcxxabi
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
    # Ubuntu has named dir as asm-generic while gcc searching for asm
    sudo ln -sv /usr/include/asm-generic/ /usr/include/asm

    # Overwrite default version of compilers
    if [ ${GCC_VERSION} == 7 ]
    then
        GCC_COMPILER=/usr/bin/gcc-7
        GPP_COMPILER=/usr/bin/g++-7
    elif [ ${GCC_VERSION} == 8 ]
    then
        GCC_COMPILER=/usr/bin/gcc-8
        GPP_COMPILER=/usr/bin/g++-8
    fi

    sudo update-alternatives --install /usr/bin/gcc gcc ${GCC_COMPILER} 1000
    sudo update-alternatives --install /usr/bin/g++ g++ ${GPP_COMPILER} 1000
fi

# setup python 3.7 as default
sudo update-alternatives --install /usr/bin/python python /usr/bin/python3.7 1000

# Install ambuild
git clone https://github.com/alliedmodders/ambuild.git ../ambuild
cd ../ambuild
sudo python setup.py install

# Build SourcePawn lib
mkdir $REPDIR/include/sourcepawn/build
cd $REPDIR/include/sourcepawn/build
python ../configure.py --enable-optimize --build=core
ambuild
cd $REPDIR