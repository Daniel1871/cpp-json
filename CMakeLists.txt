cmake_minimum_required(VERSION 3.14)
project(cpp-json)
set(CMAKE_CXX_STANDARD 20)
add_executable(main src/main.cpp)

file(GLOB JSON_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/json-examples/*.json")
foreach(FILE ${JSON_FILES})
    configure_file(${FILE} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
endforeach()
