@echo off
mkdir "../../../../Build/Win32"
cd "../../../../Build/Win32"
cmake -G "Visual Studio 17 2022" -A Win32 ../../Source