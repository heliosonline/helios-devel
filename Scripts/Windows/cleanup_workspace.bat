@echo off


echo ===== Cleanup Visual Studio 2022 workspace...
pushd ..\..\

echo Removing folder: ".\_bin"
rmdir /s /q bin

echo Removing folder: ".\_build"
rmdir /s /q build

popd


echo ===== Done.
pause
