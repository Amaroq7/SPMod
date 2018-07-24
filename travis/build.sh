#!/bin/bash
mkdir -p build
cd build
meson .. . --buildtype release --warnlevel 3 -D linktype=$LINK_TYPE
meson configure
ninja -v
