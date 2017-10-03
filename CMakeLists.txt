# Define project
cmake_minimum_required(VERSION 2.6)
project(sfmlproject)

# Set compiler flags
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic -O2 -g \
    -isystem ${CMAKE_SOURCE_DIR}/lib")

# Glob sources
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.h)

# Include libraries
include_directories(${CMAKE_SOURCE_DIR}/lib)
set(LIBS_SOURCES "")

# Find SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED system window graphics network audio)
if(NOT SFML_FOUND)
    message(FATAL_ERROR "Could not find SFML 2.X")
endif()
include_directories(${SFML_INCLUDE_DIR})

# Define executable
add_executable(sfmlproject ${SOURCES} ${LIBS_SOURCES})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# Link libraries
target_link_libraries(sfmlproject m)
target_link_libraries(sfmlproject dl)
target_link_libraries(sfmlproject ${SFML_LIBRARIES})

