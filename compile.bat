@echo off
set buildFolder=build
set nazwaProjektu=someApp.exe

echo Kompilacja shaderow ...
cd %~dp0resources\shaders\src\
call compile.bat  < nul 
echo.

echo Uruchamianie kompilacji CMake ... 
cd %~dp0%buildFolder%
cmake --build .

echo Uruchamianie projektu bin ... 
cd %~dp0\bin 
start %nazwaProjektu%

