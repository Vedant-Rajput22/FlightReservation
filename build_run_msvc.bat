@echo off
setlocal
REM ====== Change this if your Qt version/folder differs ======
set QT_DIR=C:\Qt\6.9.1\mingw_64
REM ===========================================================

if not exist "%QT_DIR%\lib\cmake\Qt6" (
  echo [ERROR] Qt not found at %QT_DIR%
  echo Update QT_DIR in this script.
  exit /b 1
)

set PATH=%QT_DIR%\bin;%PATH%

cmake -S . -B build-vs -G "Visual Studio 17 2022" -A x64 ^
  -DCMAKE_PREFIX_PATH="%QT_DIR%\lib\cmake\Qt6"
if errorlevel 1 exit /b 1

cmake --build build-vs --config Debug -j
if errorlevel 1 exit /b 1

build-vs\Debug\frs.exe
endlocal
