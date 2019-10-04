#!/bin/bash
mkdir -p build
cd build
python /usr/bin/meson .. . --buildtype release --werror --optimization 3 --warnlevel 3 -D linktype=$LINK_TYPE -D b_lto=true -D cpp_std=c++17 -D c_std=c11
python /usr/bin/meson configure
ninja -v
