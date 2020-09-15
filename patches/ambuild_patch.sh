#!/bin/bash

PATCH_EXEC=${1}
AMBUILD_SOURCE_DIR=${2}
CMAKE_SOURCE_DIR=${3}

${PATCH_EXEC} -N --verbose -d ${AMBUILD_SOURCE_DIR}/ambuild2/frontend/v2_2/cpp < ${CMAKE_SOURCE_DIR}/patches/gcc-nodbg-clang.patch || true
