# guest/CMakeLists.txt — Shared library (hot-reloadable)

add_library(cui_guest SHARED
    src/api.cpp
    src/ui_main.cpp
)

target_include_directories(cui_guest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(cui_guest PRIVATE
    cui_core
    cui_ui
)

# Ensure consistent output naming
set_target_properties(cui_guest PROPERTIES
    PREFIX ""                              # No "lib" prefix on Linux
    OUTPUT_NAME "cui_guest"
)

# DLL export definitions for MSVC
if(MSVC)
    target_compile_definitions(cui_guest PRIVATE CUI_GUEST_EXPORTS)
endif()

# Make sure the guest DLL is always built (even in Release)
set_target_properties(cui_guest PROPERTIES
    BUILD_RPATH_USE_ORIGIN TRUE
)
