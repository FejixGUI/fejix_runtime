# Hi! You are looking at the CMake script for building Fejix
# Please, do not do crazy stuff here.
# This project should support different build systems 

cmake_minimum_required(VERSION 3.0)

include(CheckIncludeFiles)

project(
    fejix_runtime
    VERSION       0.0.1
    DESCRIPTION   "Widget management framework"
    HOMEPAGE_URL  "https://fejix.dev"
    LANGUAGES     "C"
)

add_library(fejix_runtime STATIC)

set(SRC ${CMAKE_SOURCE_DIR})

include("${SRC}/tools/cmake/detect_platform.cmake")
include("${SRC}/tools/cmake/configure_output.cmake")

# You should always #include <fejix_runtime/...>
target_include_directories(fejix_runtime PUBLIC "include")

# Let it decide what C files to use
include("${SRC}/tools/cmake/select_sources.cmake")
include("${SRC}/tools/cmake/add_deps.cmake")




option(BUILD_DEVTEST "Shall I build the development-only test?" OFF)

if(BUILD_DEVTEST)
    add_executable(fejix_test "test/main.c")
    target_link_libraries(fejix_test fejix_runtime)
endif()