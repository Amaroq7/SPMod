$VS_CRT_TYPE = If ($env:LINK_TYPE -eq "dynamic") {"md"} Else {"mt"}

Start-Process "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat"
mkdir build
cd build
meson .. . --buildtype release --werror --warnlevel 3 --optimization 3 -D b_vscrt=$VS_CRT_TYPE -D linktype=$env:LINK_TYPE -D b_ndebug=true -D cpp_std=c++17 -D c_std=c11
meson configure
ninja -v
cd ..
move build\src\spmod_mm.dll dlls
