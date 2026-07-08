# examples/hotreload/CMakeLists.txt

add_executable(cui_hotreload hotreload.cpp)
target_link_libraries(cui_hotreload PRIVATE cui_core cui_ui)
target_compile_features(cui_hotreload PRIVATE cxx_std_23)

# Copy guest DLL to output directory
add_custom_command(TARGET cui_hotreload POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:cui_guest>
        $<TARGET_FILE_DIR:cui_hotreload>
    COMMENT "Copying cui_guest DLL to output directory"
)
