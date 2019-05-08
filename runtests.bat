@echo off
set olddir=%CD%
cd /D "%~dp0"

IF NOT EXIST build/tests/Debug/metaxxa-tests.exe (
    call rebuild.bat
)

call build.bat
call "build/tests/Debug/tefri-tests"

cd /D %olddir%