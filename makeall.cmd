@echo off
echo.
echo Building executables - "Win32 Unicode Debug"...
echo.
msdev.com xsubst.dsw /MAKE "WinSubst - Win32 Unicode Debug" /REBUILD
echo.
echo Building executables - "Win32 Unicode Release"...
echo.
msdev.com xsubst.dsw /MAKE "WinSubst - Win32 Unicode Release" /REBUILD
echo.
