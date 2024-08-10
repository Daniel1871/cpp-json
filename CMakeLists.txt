cmake_minimum_required(VERSION 3.14)
project(cpp-json)
add_executable(main src/main.cpp)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/file.json
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
