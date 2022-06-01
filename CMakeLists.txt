cmake_minimum_required(VERSION 3.10)

project(Tetris)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(SOURCES src/main.cpp)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_compile_options(-lsfml-graphics -lsfml-window -lsfml-system)

add_executable(Tetris ${SOURCES})
