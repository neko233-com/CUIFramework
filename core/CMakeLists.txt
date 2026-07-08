# core/CMakeLists.txt — Static library, never reloaded

# GLAD OpenGL loader — generate or download
# For simplicity, we use a minimal GLAD setup
set(GLAD_DIR "${CMAKE_BINARY_DIR}/_deps/glad")
file(MAKE_DIRECTORY "${GLAD_DIR}/include/glad")
file(MAKE_DIRECTORY "${GLAD_DIR}/src")

# Generate minimal glad.h
file(WRITE "${GLAD_DIR}/include/glad/glad.h" [[
#ifndef GLAD_H
#define GLAD_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void* (*GLADloadfunc)(const char* name);

int gladLoadGL(GLADloadfunc load);

#ifdef __cplusplus
}
#endif

#endif // GLAD_H
]])

# Minimal glad.c stub (will be replaced by proper GLAD generation)
file(WRITE "${GLAD_DIR}/src/glad.c" [[
#include <glad/glad.h>
#ifdef _WIN32
#include <windows.h>
#endif

static GLADloadfunc glad_load = 0;

void* glad_get_proc_address(const char* name) {
#ifdef _WIN32
    return (void*)GetProcAddress((HMODULE)GetModuleHandleA("opengl32.dll"), name);
#else
    return 0;
#endif
}

int gladLoadGL(GLADloadfunc load) {
    glad_load = load ? load : glad_get_proc_address;
    return 1;
}
]])

add_library(glad STATIC "${GLAD_DIR}/src/glad.c")
target_include_directories(glad PUBLIC "${GLAD_DIR}/include")

# Core static library
add_library(cui_core STATIC
    src/app.cpp
    src/window.cpp
    src/renderer.cpp
    src/input.cpp
    src/hot_reload.cpp
)

target_include_directories(cui_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(cui_core PUBLIC
    SDL2::SDL2
    glad
)

target_compile_features(cui_core PUBLIC cxx_std_23)

# DLL export macro for MSVC
if(MSVC)
    target_compile_definitions(cui_core PRIVATE CUI_BUILDING_DLL)
endif()
