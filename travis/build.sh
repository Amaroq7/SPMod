#!/bin/bash
mkdir -p build
cd build
python3.6 /usr/bin/meson .. . --buildtype release --warnlevel 3 -D dynamic_link=true
python3.6 /usr/bin/meson configure
ninja -v
