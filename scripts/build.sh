#!/bin/bash
# ============================================================================
# CUIFramework Build Script for Linux/macOS
# ============================================================================
# Usage:
#   scripts/build.sh [preset] [config]
#   scripts/build.sh linux-x64-release
#   scripts/build.sh linux-arm64-debug
#   scripts/build.sh macos-arm64-release
# ============================================================================

set -e

PRESET="${1:-linux-x64-release}"
CONFIG="${2:-Release}"

echo "============================================================================"
echo "CUIFramework Build"
echo "Preset: $PRESET"
echo "Config: $CONFIG"
echo "============================================================================"

# Configure
echo ""
echo "[1/3] Configuring..."
cmake --preset "$PRESET"

# Build
echo ""
echo "[2/3] Building..."
cmake --build --preset "$PRESET" --config "$CONFIG" -j$(nproc 2>/dev/null || sysctl -n hw.ncpu)

# Package (optional)
echo ""
echo "[3/3] Packaging..."
cmake --build --preset "$PRESET" --config "$CONFIG" --target package || echo "WARNING: Packaging skipped"

echo ""
echo "============================================================================"
echo "Build complete! Output directories:"
echo "  Binary:  build/${PRESET}/bin/${CONFIG}/"
echo "  Library: build/${PRESET}/lib/${CONFIG}/"
echo "============================================================================"

# Show output
ls -la build/${PRESET}/bin/${CONFIG}/ 2>/dev/null || true
