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
    RUNTIME_OUTPUT_DIRECTORY            "${CMAKE_SOURCE_DIR}/output"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG      "${CMAKE_SOURCE_DIR}/output"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE    "${CMAKE_SOURCE_DIR}/output")

target_link_libraries(${PROJECT_NAME} PUBLIC SDL3_ttf::SDL3_ttf SDL3_image::SDL3_image SDL3::SDL3)


if (NOT WINDOW_HEIGHT OR NOT ${WINDOW_HEIGHT})
    SET(WINDOW_HEIGHT 600)
endif()

if (NOT WINDOW_WIDTH OR NOT ${WINDOW_WIDTH})
    SET(WINDOW_WIDTH 800)
endif()

message("Window Height = ${WINDOW_HEIGHT}")
message("Window Width = ${WINDOW_WIDTH}")

ADD_DEFINITIONS(-DWINDOW_HEIGHT=${WINDOW_HEIGHT})
ADD_DEFINITIONS(-DWINDOW_WIDTH=${WINDOW_WIDTH})

if (LINUX)
    SET(SDL_IMAGE_SHARED_PATH "${CMAKE_BINARY_DIR}/sdl/SDL_image/libSDL3_image.so.0")
    SET(SDL_TTF_SHARED_PATH "${CMAKE_BINARY_DIR}/sdl/SDL_ttf/libSDL3_ttf.so.0")
    SET(SDL_SHARED_PATH "${CMAKE_BINARY_DIR}/sdl/SDL/libSDL3.so.0")
else()
    SET(SDL_IMAGE_SHARED_PATH "${CMAKE_BINARY_DIR}/sdl/SDL_image/Release/SDL3_image.dll")
    SET(SDL_TTF_SHARED_PATH "${CMAKE_BINARY_DIR}/sdl/SDL_ttf/Release/SDL3_ttf.dll")
    SET(SDL_SHARED_PATH "${CMAKE_BINARY_DIR}/sdl/SDL/Release/SDL3.dll")
endif()

SET(ASSETS_PATH ${CMAKE_SOURCE_DIR}/assets)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${ASSETS_PATH}" "${CMAKE_SOURCE_DIR}/output/assets"
    COMMENT "Copying assets directory")
    
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${SDL_SHARED_PATH}" "${CMAKE_SOURCE_DIR}/output"
    COMMAND ${CMAKE_COMMAND} -E copy "${SDL_IMAGE_SHARED_PATH}" "${CMAKE_SOURCE_DIR}/output"
    COMMAND ${CMAKE_COMMAND} -E copy "${SDL_TTF_SHARED_PATH}" "${CMAKE_SOURCE_DIR}/output"
    COMMENT "Copying shared libraries")

if (LINUX)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/run_game.sh" "${CMAKE_SOURCE_DIR}/output"
        COMMENT "Copying run script")
endif()
        
unset(WINDOW_HEIGHT CACHE)
unset(WINDOW_WIDTH CACHE)
