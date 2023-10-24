
set(HEADER_FILES ${CMAKE_SOURCE_DIR}/src/snake.h
                 ${CMAKE_SOURCE_DIR}/src/apple.h)
set(SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/main.c 
                 ${CMAKE_SOURCE_DIR}/src/snake.c
                 ${CMAKE_SOURCE_DIR}/src/apple.c)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME}
PROPERTIES
RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin")

target_link_libraries(${PROJECT_NAME} PUBLIC SDL3::SDL3)