cmake_minimum_required(VERSION 3.5)

# Project Name
PROJECT(generator)

add_executable(${PROJECT_NAME} generator.cpp)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})