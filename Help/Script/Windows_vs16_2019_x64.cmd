@echo off
mkdir "../../../../Build/Win_x64"
cd "../../../../Build/Win_x64"
cmake -G "Visual Studio 16 2019" -A x64 ../../Source