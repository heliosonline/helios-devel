@echo off


echo ===== Cleanup Visual Studio 2022 workspace...
pushd ..\..\

echo Removing folder: ".\bin"
rmdir /s /q bin

echo Removing folder: ".\build"
rmdir /s /q build

echo Removing folder: ".\tools\build_inc\bin"
rmdir /s /q tools\build_inc\bin

popd


echo ===== Done.
pause
