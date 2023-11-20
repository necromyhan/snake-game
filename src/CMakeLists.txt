
set(HEADER_FILES ${CMAKE_SOURCE_DIR}/src/snake.h
                 ${CMAKE_SOURCE_DIR}/src/apple.h
                 ${CMAKE_SOURCE_DIR}/src/font.h
                 ${CMAKE_SOURCE_DIR}/src/tileset.h
                 ${CMAKE_SOURCE_DIR}/src/menu.h
                 ${CMAKE_SOURCE_DIR}/src/game.h)

set(SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/main.c 
                 ${CMAKE_SOURCE_DIR}/src/snake.c
                 ${CMAKE_SOURCE_DIR}/src/apple.c
                 ${CMAKE_SOURCE_DIR}/src/font.c
                 ${CMAKE_SOURCE_DIR}/src/game.c
                 ${CMAKE_SOURCE_DIR}/src/menu.c
                 ${CMAKE_SOURCE_DIR}/src/tileset.c)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME}
PROPERTIES
RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output")

target_link_libraries(${PROJECT_NAME} PUBLIC SDL3_ttf::SDL3_ttf SDL3_image::SDL3_image SDL3::SDL3)

SET(ASSETS_PATH ${CMAKE_SOURCE_DIR}/assets)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${ASSETS_PATH}" "${CMAKE_SOURCE_DIR}/output/assets"
    COMMENT "Copying assets directory")