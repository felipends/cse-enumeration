cmake_minimum_required(VERSION 3.20)
project(enumeration)

find_package(ortools CONFIG REQUIRED)

set(SOURCE_FILES
    ./src/main.cpp
    ./src/instance/instance.hpp
    ./src/instance/instance.cpp
    ./src/instancesGenerator/instancesGenerator.cpp
    ./src/instancesGenerator/instancesGenerator.hpp
    ./src/model/model.hpp
    ./src/model/model.cpp
)

add_compile_options(-O3 -Wall -Wextra -Wpedantic)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ortools::ortools)