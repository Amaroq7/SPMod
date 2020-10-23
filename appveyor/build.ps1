$BUILD_DYNAMIC = If ($env:LINK_TYPE -eq "dynamic") {"ON"} Else {"OFF"}

Start-Process "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat"
New-Item $env:APPVEYOR_BUILD_FOLDER\build -itemtype directory -Force -ErrorAction SilentlyContinue
Set-Location -Path $env:APPVEYOR_BUILD_FOLDER\build
cmake -G Ninja --log-level=VERBOSE -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../upload -DBUILD_DYNAMIC=$BUILD_DYNAMIC .. .
ninja install -v
