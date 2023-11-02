
set(HEADER_FILES ${CMAKE_SOURCE_DIR}/src/snake.h
                 ${CMAKE_SOURCE_DIR}/src/apple.h
                 ${CMAKE_SOURCE_DIR}/src/font.h
                 ${CMAKE_SOURCE_DIR}/src/field.h)

set(SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/main.c 
                 ${CMAKE_SOURCE_DIR}/src/snake.c
                 ${CMAKE_SOURCE_DIR}/src/apple.c
                 ${CMAKE_SOURCE_DIR}/src/font.c
                 ${CMAKE_SOURCE_DIR}/src/field.c)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME}
PROPERTIES
RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output/bin")

target_link_libraries(${PROJECT_NAME} PUBLIC SDL3_ttf::SDL3_ttf SDL3::SDL3)