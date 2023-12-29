cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(vector VERSION 0.1.0 LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME}
    main.c
    vector.h
)