# ui/CMakeLists.txt — Static library, linked into guest

add_library(cui_ui STATIC
    src/builder.cpp
    src/layout.cpp
    src/components.cpp
)

target_include_directories(cui_ui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(cui_ui PUBLIC cui_core)

target_compile_features(cui_ui PUBLIC cxx_std_23)
