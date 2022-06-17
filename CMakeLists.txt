cmake_minimum_required(VERSION 3.20)
project(TicTacToe VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)
set(SDL2_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)
set(SDL2_IMAGE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2_image)
set(SDL2_TTF_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2_ttf)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(TicTacToe main.cpp SDLUtil.h SDLUtil.cpp main.h Textures.h Textures.cpp Fonts.h Fonts.cpp)
target_link_libraries(TicTacToe PRIVATE SDL2::Main SDL2::Image SDL2::TTF)
