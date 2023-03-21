cmake_minimum_required(VERSION 3.0.0)
project(snake VERSION 0.1.0)

add_executable(snake main.c)

target_link_libraries(snake SDL2main SDL2)
