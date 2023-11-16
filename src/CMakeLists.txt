
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

SET(TILE_SHEET_PATH ${CMAKE_SOURCE_DIR}/assets/snake_tile_sheet.png)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${TILE_SHEET_PATH}" "${CMAKE_SOURCE_DIR}/output"
    COMMENT "Copying tilesheet to output directory")

SET(FONT_PATH ${CMAKE_SOURCE_DIR}/assets/kongtext.regular.ttf)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${FONT_PATH}" "${CMAKE_SOURCE_DIR}/output"
    COMMENT "Copying font file to output directory")