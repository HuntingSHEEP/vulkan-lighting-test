@echo off 

cd %~dp0
call build.bat

echo.
cd %~dp0
call compile.bat