rem @echo off
echo on

cd %APPVEYOR_BUILD_FOLDER%
echo ARCH: %ARCH%

rem TODO cygwin pkg and src tar.xz
if "%CYGWIN%" == "1" du -sh
if "%CYGWIN%" == "1" exit

rem simple windows .zip with bins and pdf in root
set DESTDIR=dieharder-%APPVEYOR_BUILD_VERSION%-%ARCH%
if not "x%APPVEYOR_REPO_TAG_NAME%" == "x" set DESTDIR=dieharder-%APPVEYOR_REPO_TAG_NAME%-%ARCH%
echo DESTDIR: %DESTDIR%

bash -c "make install prefix= bindir= pdfdir= DESTDIR=`pwd`/%DESTDIR%"
rem TODO install-pdf broken
rem bash -c "make install-pdf prefix= bindir= pdfdir= DESTDIR=`pwd`/%DESTDIR%"
cd %DESTDIR%
copy ..\README README.txt
move /y libdieharder\libdieharder-3.dll libdieharder-3.dll
rem libssp-0.dll needed for -fstack-protector
copy c:\%MSYS2_DIR%\%MSYSTEM%\bin\libssp-0.dll *.*
copy ..\dieharder\.libs\*.exe *.*
7z a ../%DESTDIR%.zip *
appveyor PushArtifact ../%DESTDIR%.zip *
cd ..
dir *.*
