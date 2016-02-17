cmake_minimum_required(VERSION 2.6)

project(uthread C)

set(CMAKE_C_FLAGS "-g")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(examples)
