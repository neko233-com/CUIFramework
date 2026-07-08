# guest/CMakeLists.txt — Shared library (hot-reloadable)

add_library(cui_guest SHARED
    src/ui_main.cpp
)

target_include_directories(cui_guest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/extern/cr/cr
)

target_link_libraries(cui_guest PRIVATE
    cui_core
    cui_ui
)

set_target_properties(cui_guest PROPERTIES
    PREFIX ""
    OUTPUT_NAME "cui_guest"
)

if(MSVC)
    target_compile_definitions(cui_guest PRIVATE CUI_GUEST_EXPORTS)
endif()
