#!/bin/bash

if [[ ! -z "${CLANG_VERSION}" ]]
then
    sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-${CLANG_VERSION} 1000
    sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-${CLANG_VERSION} 1000
    sudo update-alternatives --install /usr/bin/lld lld /usr/bin/lld-${CLANG_VERSION} 1000
elif [[ ! -z "${GCC_VERSION}" ]]
then
    # Ubuntu has named dir as asm-generic while gcc searching for asm
    sudo ln -sv /usr/include/asm-generic/ /usr/include/asm

    # Overwrite default version of compilers
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-$GCC_VERSION 1000
    sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-$GCC_VERSION 1000
fi

# setup python 3.6 as default
sudo update-alternatives --install /usr/bin/python python /usr/bin/python3 1000
