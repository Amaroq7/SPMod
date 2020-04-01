$BUILD_DYNAMIC = If ($env:LINK_TYPE -eq "dynamic") {"ON"} Else {"OFF"}

Start-Process "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat"
New-Item $env:APPVEYOR_BUILD_FOLDER\build -itemtype directory -Force -ErrorAction SilentlyContinue
Set-Location -Path $env:APPVEYOR_BUILD_FOLDER\build
cmake -T v142,host=x64 -G "Visual Studio 16 2019" -A Win32 -D BUILD_DYNAMIC=$BUILD_DYNAMIC -D CMAKE_INSTALL_PREFIX=../upload ..
cmake --build . --config Release --target INSTALL