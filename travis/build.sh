#!/bin/bash

if [[ $LINK_TYPE == "dynamic" ]]
then
    BUILD_DYNAMIC="ON"
else
    BUILD_DYNAMIC="OFF"
fi

mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../upload -DBUILD_DYNAMIC=${BUILD_DYNAMIC} .. .
make -j 16 install
