@echo off
echo.
echo Building executables...
echo.
msdev.com xsubst.dsw /MAKE "WinSubst - Win32 Unicode Release" /REBUILD
echo.
echo.
echo Creating installer...
echo.
if exist .\Setup\*.exe del .\Setup\*.exe
iscc.exe Setup.iss
