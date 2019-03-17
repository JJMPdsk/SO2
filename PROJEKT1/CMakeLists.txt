cmake_minimum_required(VERSION 3.10)
project(PROJEKT1)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-lncurses")
target_link_libraries(${PROJEKT1} ncurses)



add_executable(PROJEKT1 main.cpp player.h cmake-build-debug/ball.h global.h)