@echo off
mkdir "../../../../Build/Win_x64"
cd "../../../../Build/Win_x64"
cmake -DCMAKE_PLATFORM=x64 -G "Visual Studio 17 2022" -A x64 ../../Source