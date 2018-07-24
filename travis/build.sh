#!/bin/bash
mkdir -p build
cd build
python /usr/bin/meson .. . --buildtype release --warnlevel 3 -D linktype=$LINK_TYPE
python /usr/bin/meson configure
ninja -v
