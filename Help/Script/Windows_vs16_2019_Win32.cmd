@echo off
mkdir "../../../../Build/Win32"
cd "../../../../Build/Win32"
cmake -G "Visual Studio 16 2019" -A Win32 ../../Source