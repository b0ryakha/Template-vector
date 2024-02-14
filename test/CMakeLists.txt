cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(vec_test VERSION 0.1.0 LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(${PROJECT_NAME}
    ../vector.h

    test.h
    main.c
)