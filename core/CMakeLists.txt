# core/CMakeLists.txt — Static library, never reloaded

# GLAD OpenGL loader
set(GLAD_SOURCES
    src/glad.c
)

# Core static library
add_library(cui_core STATIC
    ${GLAD_SOURCES}
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
    SDL2_ttf::SDL2_ttf
)

target_compile_features(cui_core PUBLIC cxx_std_23)

# DLL export macro for MSVC
if(MSVC)
    target_compile_definitions(cui_core PRIVATE CUI_BUILDING_DLL)
endif()
