@echo off

pushd ..\
call Dependencies\Premake\premake5.exe vs2019
popd
PAUSE