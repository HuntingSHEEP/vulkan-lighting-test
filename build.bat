@echo off 
set buildFolder=build 

if exist %buildFolder% (
    rd /s /q %buildFolder% 
    echo Usunieto folder %buildFolder%
)

mkdir %buildFolder% 
echo Utworzono folder %buildFolder%

echo Tworzenie projektu CMake ... 
echo .
cd %~dp0%buildFolder% 
cmake .. 

