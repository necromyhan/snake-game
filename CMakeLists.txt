cmake_minimum_required(VERSION 3.27)
project(test_game)

set(CMAKE_C_STANDART C99)

add_subdirectory(src)
add_subdirectory(sdl/SDL)
add_subdirectory(sdl/SDL_ttf)