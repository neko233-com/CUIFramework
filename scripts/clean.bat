@echo off
REM ============================================================================
REM CUIFramework Clean Build Script for Windows
REM ============================================================================

echo Cleaning build directories...
if exist build (
    rmdir /s /q build
    echo Build directory cleaned.
) else (
    echo No build directory to clean.
)

echo Done.
