#!/bin/bash
# ============================================================================
# CUIFramework — 一键环境安装脚本 (Linux/macOS)
# ============================================================================
# 该脚本自动安装构建 CUIFramework 所需的所有依赖。
#
# 使用方法:
#   chmod +x scripts/setup-unix.sh
#   scripts/setup-unix.sh
# ============================================================================

set -e

echo "============================================================================"
echo " CUIFramework 环境安装脚本"
echo "============================================================================"
echo ""

# 检测操作系统
detect_os() {
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        echo "linux"
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        echo "macos"
    else
        echo "unknown"
    fi
}

OS=$(detect_os)
echo "检测到操作系统: $OS"
echo ""

# ---- 1. 安装包管理器依赖 ----
echo "[1/5] 安装系统依赖..."

if [ "$OS" = "linux" ]; then
    # 检测发行版
    if command -v apt-get &> /dev/null; then
        echo "  使用 apt (Debian/Ubuntu)..."
        sudo apt-get update
        sudo apt-get install -y \
            build-essential \
            cmake \
            git \
            libgl1-mesa-dev \
            libglu1-mesa-dev \
            libx11-dev \
            libxext-dev \
            libxrandr-dev \
            libxcursor-dev \
            libxi-dev \
            libxss-dev \
            libasound2-dev \
            libpulse-dev \
            libudev-dev \
            pkg-config
    elif command -v dnf &> /dev/null; then
        echo "  使用 dnf (Fedora/RHEL)..."
        sudo dnf install -y \
            gcc-c++ \
            cmake \
            git \
            mesa-libGL-devel \
            mesa-libGLU-devel \
            libX11-devel \
            libXext-devel \
            libXrandr-devel \
            libXcursor-devel \
            libXi-devel \
            libXss-devel \
            alsa-lib-devel \
            pulseaudio-libs-devel \
            systemd-devel \
            pkg-config
    elif command -v pacman &> /dev/null; then
        echo "  使用 pacman (Arch)..."
        sudo pacman -S --needed \
            base-devel \
            cmake \
            git \
            mesa \
            libglvnd \
            libx11 \
            libxext \
            libxrandr \
            libxcursor \
            libxi \
            libxss \
            alsa-lib \
            libpulse \
            systemd \
            pkg-config
    else
        echo "  警告: 未知的包管理器，请手动安装依赖"
    fi

elif [ "$OS" = "macos" ]; then
    echo "  使用 brew (macOS)..."
    if ! command -v brew &> /dev/null; then
        echo "  安装 Homebrew..."
        /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    fi

    brew install cmake git pkg-config
fi

# ---- 2. 检查 CMake ----
echo "[2/5] 检查 CMake..."
if command -v cmake &> /dev/null; then
    echo "  CMake 版本: $(cmake --version | head -1)"
else
    echo "  错误: CMake 未安装"
    exit 1
fi

# ---- 3. 检查编译器 ----
echo "[3/5] 检查 C++ 编译器..."
if command -v g++ &> /dev/null; then
    echo "  g++ 版本: $(g++ --version | head -1)"
elif command -v clang++ &> /dev/null; then
    echo "  clang++ 版本: $(clang++ --version | head -1)"
else
    echo "  错误: 未找到 C++ 编译器"
    exit 1
fi

# ---- 4. 初始化子模块 ----
echo "[4/5] 初始化 Git 子模块..."
if [ -d .git ]; then
    git submodule update --init --recursive
else
    echo "  警告: 不在 Git 仓库中，跳过"
fi

# ---- 5. 配置 CMake ----
echo "[5/5] 配置 CMake..."
if [ "$OS" = "linux" ]; then
    cmake --preset linux-x64-debug
elif [ "$OS" = "macos" ]; then
    # 检测架构
    ARCH=$(uname -m)
    if [ "$ARCH" = "arm64" ]; then
        cmake --preset macos-arm64-debug
    else
        cmake --preset macos-x64-debug
    fi
fi

echo ""
echo "============================================================================"
echo " 环境安装完成！"
echo "============================================================================"
echo ""
echo " 后续操作:"
echo "   构建:  cmake --build --preset linux-x64-debug"
echo "   运行:  ./build/linux-x64/bin/Debug/cui_basic"
echo "   热重载: ./build/linux-x64/bin/Debug/cui_hotreload"
echo ""
