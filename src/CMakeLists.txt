add_executable(test_game main.c)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL3::SDL3)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin"
)