option(FEJIX_USE_X11 "Use X11 protocol for window management (Usually, targeting GNU/Linux distros)" OFF)
option(FEJIX_USE_WINAPI "Use Windows API for window management (Usually, targeting MS Windows)" OFF)
option(FEJIX_USE_VARSIZE "Enable VarSize feature" ON)
option(FEJIX_USE_OPENGL "Include functionality for setting up OpenGL" OFF)
option(FEJIX_USE_OPENGL_3 "If FEJIX_USE_OPENGL in ON, signals that OpenGL 3 must be used" OFF)


if(FEJIX_USE_WINAPI)
    check_include_files("windows.h" FEJIX_HAS_WINAPI)
    if(NOT FEJIX_HAS_WINAPI)
        message(FATAL_ERROR "No 'windows.h' found!")
    endif()

    target_compile_definitions(fejix_runtime PUBLIC "FJ_USE_WINAPI")

    if(FEJIX_USE_VARSIZE)
        target_compile_definitions(fejix_runtime PUBLIC "FJ_USE_VARSIZE")
    endif()
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

    if(FEJIX_USE_VARSIZE)
        target_compile_definitions(fejix_runtime PUBLIC "FJ_USE_VARSIZE")
    endif()
endif()


if(FEJIX_USE_OPENGL)
    target_compile_definitions(fejix_runtime PUBLIC "FJ_USE_OPENGL")

    if(FEJIX_USE_OPENGL_3)
        target_compile_definitions(fejix_runtime PUBLIC "FJ_USE_OPENGL_3")
    endif()

    # The list is to be extended in the future
    if(NOT FEJIX_USE_OPENGL_3)
        message(FATAL_ERROR "Please, select one of the following OpenGL versions: 3")
    endif()
endif()