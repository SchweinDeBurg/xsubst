@echo off

echo.
echo Building executables - "Win32 Unicode Debug"...
echo.
devenv.com xsubst71.sln /rebuild "Unicode Debug" /project WinSubst /nologo

echo.
echo Building executables - "Win32 Unicode Release"...
echo.
devenv.com xsubst71.sln /rebuild "Unicode Release" /project WinSubst /nologo

echo.
