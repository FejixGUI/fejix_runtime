# Hi! You are looking at the CMake script for building Fejix Runtime.
# Do not do crazy stuff.
# This project should be as easy to build as possible.

cmake_minimum_required(VERSION 3.0)

include(CheckIncludeFiles)

project(
    fejix_runtime
    VERSION       0.0.1
    DESCRIPTION   "Widget management framework"
    HOMEPAGE_URL  "https://github.com/FejixGUI/fejix_runtime"
    LANGUAGES     "C"
)

add_library(fejix_runtime STATIC)

set(ROOT ${CMAKE_SOURCE_DIR})

# Adds the appropriate definitions and check for build requirements
include("${ROOT}/tools/cmake/process_options.cmake")

# Configure output
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Let it decide what C files to use
include("${ROOT}/tools/cmake/add_sources.cmake")

# Add dependencies
include("${ROOT}/tools/cmake/add_deps.cmake")


# === TESTS ===

option(BUILD_DEVTEST "The executable I use to debug Fejix Runtime" OFF)

if(BUILD_DEVTEST)
    add_executable(fejix_devtest "test/main.c")
    target_sources(fejix_devtest PRIVATE "test/debug.c")
    target_include_directories(fejix_devtest PRIVATE "test")
    target_link_libraries(fejix_devtest fejix_runtime)
endif()