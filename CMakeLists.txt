# Hi! You are looking at the CMake script for building Fejix.
# Please, do not do crazy stuff here.
# This project should be easy to build, so that alternative build systems may be used.

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

# You should always #include <fejix_runtime/...> for public headers
# You should always #include <fejix_private/...> for private headers
target_include_directories(fejix_runtime PUBLIC "include")

# Let it decide what C files to use
include("${ROOT}/tools/cmake/add_sources.cmake")

# Add dependencies
include("${ROOT}/tools/cmake/add_deps.cmake")


# === TESTS ===

option(BUILD_DEVTEST "Shall I build the development-only test?" OFF)

if(BUILD_DEVTEST)
    add_executable(fejix_test "test/main.c")
    target_sources(fejix_test PRIVATE "test/debug.c")
    target_include_directories(fejix_test PRIVATE "test")
    target_link_libraries(fejix_test fejix_runtime)
endif()