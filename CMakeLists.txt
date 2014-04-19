cmake_minimum_required(VERSION 2.6)
project(pmr)
set(CMAKE_INCLUDE_CURRENT_DIR True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules ${CMAKE_PREFIX_PATH}/share/apps/cmake/modules)

find_package(SFML REQUIRED COMPONENTS graphics window system)
include_directories(${SFML_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS_RELEASE "-std=c++11 -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g -std=c++11 -Wall")

add_subdirectory(src)
