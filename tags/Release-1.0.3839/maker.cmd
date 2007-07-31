@echo off

echo.
echo Building executables...
echo.
devenv.com xsubst71.sln /rebuild "Unicode Release" /project WinSubst /nologo

echo.
echo.
echo Creating standard installer...
echo.
iscc.exe Setup.iss

echo.
echo.
echo Creating universal installer...
echo.
iscc.exe SetupUniversal.iss
