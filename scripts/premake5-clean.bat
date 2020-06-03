@echo off

pushd ..\
call Dependencies\Premake\premake5.exe clean
popd
PAUSE