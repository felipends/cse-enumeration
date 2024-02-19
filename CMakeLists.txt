cmake_minimum_required(VERSION 3.20)
project(enumeration)

set(SOURCE_FILES
    ./src/main.cpp
    ./src/instance/instance.hpp
    ./src/instance/instance.cpp
    )

add_executable(${PROJECT_NAME} ${SOURCE_FILES})