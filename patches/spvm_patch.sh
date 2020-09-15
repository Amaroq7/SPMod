#!/bin/bash

PATCH_EXEC=${1}
SPVM_SOURCE_DIR=${2}
CMAKE_SOURCE_DIR=${3}

${PATCH_EXEC} -Ns --verbose -d ${SPVM_SOURCE_DIR} < ${CMAKE_SOURCE_DIR}/patches/AMBuildScript.patch || true
${PATCH_EXEC} -Ns --verbose -d ${SPVM_SOURCE_DIR} < ${CMAKE_SOURCE_DIR}/patches/configurepy.patch || true
${PATCH_EXEC} -Ns --verbose -d ${SPVM_SOURCE_DIR}/vm < ${CMAKE_SOURCE_DIR}/patches/AMBuilder_VM_fPIC.patch || true
${PATCH_EXEC} -Ns --verbose -d ${SPVM_SOURCE_DIR}/third_party/zlib < ${CMAKE_SOURCE_DIR}/patches/AMBuilder_zlib_fPIC.patch || true
