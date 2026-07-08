cmake_minimum_required(VERSION 3.28)
project(CUIFramework VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================================
# Build output directories — build/{platform}/{arch}/{config}/
# ============================================================================
if(WIN32)
    set(PLATFORM_DIR "windows")
elseif(APPLE)
    set(PLATFORM_DIR "macos")
elseif(UNIX)
    set(PLATFORM_DIR "linux")
else()
    set(PLATFORM_DIR "unknown")
endif()

# Detect architecture
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|ARM64|arm64")
        set(ARCH_DIR "arm64")
    else()
        set(ARCH_DIR "x64")
    endif()
else()
    set(ARCH_DIR "x86")
endif()

set(BUILD_PLATFORM_DIR "${PLATFORM_DIR}-${ARCH_DIR}")

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Per-configuration output (adds /Debug or /Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/Debug")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/lib/Debug")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/lib/Release")

message(STATUS "Platform: ${PLATFORM_DIR}")
message(STATUS "Architecture: ${ARCH_DIR}")
message(STATUS "Build directory: ${BUILD_PLATFORM_DIR}")

# ============================================================================
# MSVC Configuration
# ============================================================================
if(MSVC)
    # Use /MD runtime (required by cr.h for DLL hot reload)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

    # Enable multiprocessor compilation
    add_compile_options(/MP)

    # Warning level
    add_compile_options(/W4)

    # For DLL exports
    add_compile_definitions(CUI_GUEST_EXPORTS)
endif()

# ============================================================================
# Compiler flags for cross-platform
# ============================================================================
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
    add_compile_options(-fPIC)
endif()

# ============================================================================
# Dependencies
# ============================================================================
include(FetchContent)

# SDL2
FetchContent_Declare(SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.30.x
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
set(SDL_SHARED ON CACHE BOOL "Build SDL2 as shared library" FORCE)
set(SDL_STATIC OFF CACHE BOOL "Build SDL2 as static library" FORCE)
FetchContent_MakeAvailable(SDL2)

# GLAD (OpenGL loader) — header-only, we include it directly
# We'll generate the glad.c file or use a pre-built one

# ============================================================================
# Subdirectories
# ============================================================================
add_subdirectory(core)
add_subdirectory(ui)
add_subdirectory(guest)
add_subdirectory(examples/basic)
add_subdirectory(examples/hotreload)

# ============================================================================
# Install configuration (for packaging)
# ============================================================================
install(TARGETS cui_guest LIBRARY DESTINATION bin RUNTIME DESTINATION bin)
install(DIRECTORY guest/include/ DESTINATION include)
install(DIRECTORY core/include/ DESTINATION include)
install(DIRECTORY ui/include/ DESTINATION include)

# ============================================================================
# CPack configuration (for .exe/.dmg/.deb packaging)
# ============================================================================
set(CPACK_PACKAGE_NAME "CUIFramework")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "C++23 Cross-platform UI Framework with hot reload")
set(CPACK_PACKAGE_VENDOR "neko233-com")

if(WIN32)
    set(CPACK_GENERATOR "NSIS;ZIP")
    set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
    set(CPACK_NSIS_DISPLAY_NAME "CUIFramework")
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop;TGZ")
elseif(UNIX)
    set(CPACK_GENERATOR "DEB;TGZ")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "neko233-com")
endif()

include(CPack)
