$VS_CRT_TYPE = If ($env:LINK_TYPE -eq "dynamic") {"md"} Else {"mt"}

Start-Process "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat"
New-Item $env:APPVEYOR_BUILD_FOLDER\build -itemtype directory -Force -ErrorAction SilentlyContinue
Set-Location -Path $env:APPVEYOR_BUILD_FOLDER\build
meson .. . --buildtype release --werror --warnlevel 3 --optimization 3 -D b_vscrt=$VS_CRT_TYPE -D linktype=$env:LINK_TYPE -D b_ndebug=true -D cpp_std=c++17 -D c_std=c11
meson configure
ninja -v
