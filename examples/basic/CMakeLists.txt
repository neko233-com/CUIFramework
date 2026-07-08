# examples/basic/CMakeLists.txt

add_executable(cui_basic basic.cpp)
target_link_libraries(cui_basic PRIVATE cui_core cui_ui)
target_compile_features(cui_basic PRIVATE cxx_std_23)

# Copy guest DLL to output directory
add_custom_command(TARGET cui_basic POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:cui_guest>
        $<TARGET_FILE_DIR:cui_basic>
    COMMENT "Copying cui_guest DLL to output directory"
)
