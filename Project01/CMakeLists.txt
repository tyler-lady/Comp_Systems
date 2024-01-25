cmake_minimum_required(VERSION 3.26)
project(Project01 C)

set(CMAKE_C_STANDARD 11)

add_executable(Project01 main.c
        controller.c
        controller.h)
