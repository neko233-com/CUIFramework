@echo off
REM ============================================================================
REM CUIFramework Build Script for Windows
REM ============================================================================
REM Usage:
REM   scripts\build.bat [preset] [config]
REM   scripts\build.bat win-x64-release
REM   scripts\build.bat win-x86-debug
REM ============================================================================

setlocal enabledelayedexpansion

set PRESET=%1
set CONFIG=%2

if "%PRESET%"=="" set PRESET=win-x64-release
if "%CONFIG%"=="" set CONFIG=Release

echo ============================================================================
echo CUIFramework Build
echo Preset: %PRESET%
echo Config: %CONFIG%
echo ============================================================================

REM Configure
echo.
echo [1/3] Configuring...
cmake --preset %PRESET%
if errorlevel 1 (
    echo ERROR: CMake configure failed!
    exit /b 1
)

REM Build
echo.
echo [2/3] Building...
cmake --build --preset %PRESET% --config %CONFIG% -j %NUMBER_OF_PROCESSORS%
if errorlevel 1 (
    echo ERROR: Build failed!
    exit /b 1
)

REM Package
echo.
echo [3/3] Packaging...
cmake --build --preset %PRESET% --config %CONFIG% --target package
if errorlevel 1 (
    echo WARNING: Packaging failed (non-critical)
)

echo.
echo ============================================================================
echo Build complete! Output in: build\windows-x64\bin\%CONFIG%\
echo ============================================================================

REM Show output directory contents
dir build\windows-x64\bin\%CONFIG%\*.exe 2>nul
dir build\windows-x64\bin\%CONFIG%\*.dll 2>nul

endlocal
