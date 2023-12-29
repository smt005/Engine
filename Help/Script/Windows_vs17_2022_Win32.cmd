@echo off
mkdir "../../../../Build/Win32"
cd "../../../../Build/Win32"
cmake -DPLATFORM="Win32" -DENABLE_PHYSIC=ON -G "Visual Studio 17 2022" -A Win32 ../../Source
