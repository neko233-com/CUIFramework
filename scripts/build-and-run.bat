@echo off
REM ============================================================================
CUIFramework — 一键构建并运行脚本 (Windows)
============================================================================
该脚本自动构建所有目标并运行基础示例。

使用方法:
  scripts\build-and-run.bat [preset]
  scripts\build-and-run.bat              (默认 win-x64-debug)
  scripts\build-and-run.bat win-x64-release
============================================================================

setlocal enabledelayedexpansion

set PRESET=%1
if "%PRESET%"=="" set PRESET=win-x64-debug

echo ============================================================================
echo  CUIFramework 一键构建
echo  预设: %PRESET%
echo ============================================================================
echo.

REM 刷新 PATH
set PATH=%PATH%;C:\Program Files\CMake\bin

REM ---- 1. 检查环境 ----
echo [1/4] 检查环境...
cmake --version >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo 错误: CMake 未安装。请运行 scripts\setup-windows.bat
    pause
    exit /b 1
)
echo   CMake: OK

where cl >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo 错误: MSVC 未安装。请安装 Visual Studio Build Tools。
    pause
    exit /b 1
)
echo   MSVC: OK

REM ---- 2. 配置 ----
echo [2/4] 配置 CMake...
cmake --preset %PRESET%
if %ERRORLEVEL% neq 0 (
    echo 错误: CMake 配置失败！
    pause
    exit /b 1
)
echo   配置完成

REM ---- 3. 构建 ----
echo [3/4] 构建项目...
cmake --build --preset %PRESET% -j %NUMBER_OF_PROCESSORS%
if %ERRORLEVEL% neq 0 (
    echo 错误: 构建失败！
    pause
    exit /b 1
)
echo   构建完成

REM ---- 4. 运行示例 ----
echo [4/4] 运行基础示例...
echo.

REM 确定输出目录
set BUILD_TYPE=Debug
echo %PRESET% | findstr /i "release" >nul
if %ERRORLEVEL% equ 0 set BUILD_TYPE=Release

set EXE_PATH=build\windows-x64\bin\%BUILD_TYPE%\cui_basic.exe

if exist "%EXE_PATH%" (
    echo 启动: %EXE_PATH%
    echo.
    start "" "%EXE_PATH%"
) else (
    echo 警告: 可执行文件不存在: %EXE_PATH%
    echo 请检查构建输出。
)

echo.
echo ============================================================================
echo  构建完成！
echo ============================================================================
echo.
echo  输出目录: build\windows-x64\bin\%BUILD_TYPE%\
echo.
echo  可用可执行文件:
echo    cui_basic.exe       - 基础示例
echo    cui_hotreload.exe   - 热重载示例
echo    cui_guest.dll       - 可热重载的 Guest DLL
echo.
echo  热重载测试:
echo    1. 运行 cui_hotreload.exe
echo    2. 编辑 guest\src\ui_main.cpp
echo    3. 保存文件
echo    4. 观察窗口自动更新
echo.
pause
