option(FEJIX_USE_X11 "Use X11 protocol for window management (Usually, targeting GNU/Linux distros)" OFF)
option(FEJIX_USE_WINAPI "Use Windows API for window management (Usually, targeting MS Windows)" OFF)
option(FEJIX_USE_OPENGL3 "Include functionality for setting up OpenGL 3" OFF)
option(FEJIX_USE_NANOVG "Use NanoVG for rendering" OFF)


if(FEJIX_USE_WINAPI)

    check_include_files("windows.h" FEJIX_HAS_WINAPI)
    if(NOT FEJIX_HAS_WINAPI)
        message(FATAL_ERROR "No windows.h found!")
    endif()

    target_compile_definitions(fejix_runtime PUBLIC "FJ_USE_WINAPI")
endif()

if(FEJIX_USE_X11)

    check_include_files("X11/Xlib.h" FEJIX_HAS_X11)
    check_include_files("xcb/xcb.h" FEJIX_HAS_XCB)
    check_include_files("X11/Xlib-xcb.h" FEJIX_HAS_XLIB_XCB)
    check_include_files("xcb/sync.h" FEJIX_HAS_XCB_SYNC)
    check_include_files("xcb/xcb_icccm.h" FEJIX_HAS_XCB_ICCCM)
    if(NOT FEJIX_HAS_X11
    OR NOT FEJIX_HAS_XCB
    OR NOT FEJIX_HAS_XLIB_XCB
    OR NOT FEJIX_HAS_XCB_SYNC
    OR NOT FEJIX_HAS_XCB_ICCCM)
        message(FATAL_ERROR "Please, install the required packages for X11 (see BUILD.md), then clear CMake cache")
    endif()

    target_compile_definitions(fejix_runtime PUBLIC "FJ_USE_X11")

endif()


if(FEJIX_USE_NANOVG)
    if(NOT FEJIX_USE_OPENGL3)
        message(FATAL_ERROR "NanoVG requires OpenGL3!")
    endif()
 
    target_compile_definitions(fejix_runtime PUBLIC "FJ_USE_NANOVG")
endif()

if(FEJIX_USE_OPENGL3)
    target_compile_definitions(fejix_runtime PUBLIC "FJ_USE_OPENGL3")
endif()