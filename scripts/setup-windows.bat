@echo off
REM ============================================================================
CUIFramework — 一键环境安装脚本 (Windows)
============================================================================
该脚本自动安装构建 CUIFramework 所需的所有依赖。
以管理员身份运行或允许 UAC 提权。

使用方法:
  scripts\setup-windows.bat
============================================================================

setlocal enabledelayedexpansion

echo ============================================================================
echo  CUIFramework 环境安装脚本
echo ============================================================================
echo.

REM ---- 1. 检查并安装 Git ----
echo [1/6] 检查 Git...
git --version >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo   Git 未安装，正在安装...
    winget install Git.Git --accept-package-agreements --accept-source-agreements
) else (
    echo   Git 已安装: 
    git --version
)

REM ---- 2. 检查并安装 CMake ----
echo [2/6] 检查 CMake...
cmake --version >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo   CMake 未安装，正在安装...
    winget install Kitware.CMake --accept-package-agreements --accept-source-agreements
) else (
    echo   CMake 已安装
)

REM ---- 3. 检查并安装 Visual Studio Build Tools ----
echo [3/6] 检查 Visual Studio Build Tools...
where cl >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo   MSVC 编译器未找到。
    echo   请安装 Visual Studio 2022 Build Tools:
    echo   https://visualstudio.microsoft.com/visual-cpp-build-tools/
    echo.
    echo   需要安装 "C++ 桌面开发" 工作负载。
    echo   按任意键打开下载页面...
    pause >nul
    start https://visualstudio.microsoft.com/visual-cpp-build-tools/
    echo.
    echo   安装完成后，重新运行此脚本。
    pause
    exit /b 1
) else (
    echo   MSVC 已安装
)

REM ---- 4. 克隆子模块 (SDL2 通过 FetchContent 自动下载) ----
echo [4/6] 初始化 Git 子模块...
if exist .git (
    git submodule update --init --recursive
) else (
    echo   警告: 不在 Git 仓库中，跳过子模块初始化
)

REM ---- 5. 创建构建目录 ----
echo [5/6] 创建构建目录...
if not exist build mkdir build

REM ---- 6. 配置 CMake ----
echo [6/6] 配置 CMake (x64 Debug)...
cmake --preset win-x64-debug
if %ERRORLEVEL% neq 0 (
    echo.
    echo 错误: CMake 配置失败！
    echo 请检查是否正确安装了 Visual Studio Build Tools。
    pause
    exit /b 1
)

echo.
echo ============================================================================
echo  环境安装完成！
echo ============================================================================
echo.
echo  后续操作:
echo    构建:  cmake --build --preset win-x64-debug
echo    运行:  build\windows-x64\bin\Debug\cui_basic.exe
echo    热重载: build\windows-x64\bin\Debug\cui_hotreload.exe
echo.
echo  构建所有预设:
echo    scripts\build.bat win-x64-debug
echo    scripts\build.bat win-x64-release
echo    scripts\build.bat win-x86-debug
echo    scripts\build.bat win-x86-release
echo.
pause
