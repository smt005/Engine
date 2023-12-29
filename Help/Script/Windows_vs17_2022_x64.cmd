@echo off
mkdir "../../../../Build/Win_x64"
cd "../../../../Build/Win_x64"
cmake -DPLATFORM="x64" -DENABLE_PHYSIC=ON -G "Visual Studio 17 2022" -A x64 ../../Source
