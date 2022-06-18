# FEJIX_USE_XXX - result of this script that indicates that XXX will be used
# FEJIX_HAS_XXX - a header XXX.h is detected

check_include_files("windows.h" FEJIX_HAS_WINAPI)
check_include_files("X11/Xlib.h" FEJIX_HAS_X11)

set(FEJIX_USE_WINAPI OFF)
set(FEJIX_USE_X11 OFF)

if(FEJIX_HAS_WINAPI)

    set(FEJIX_USE_WINAPI ON)

    target_compile_definitions(fejix_runtime PUBLIC "FJ_USE_WINAPI")

elseif(FEJIX_HAS_X11)

    set(FEJIX_USE_X11 ON)

    check_include_files("xcb/xcb.h" FEJIX_HAS_XCB)
    check_include_files("X11/Xlib-xcb.h" FEJIX_HAS_XLIB_XCB)
    check_include_files("xcb/sync.h" FEJIX_HAS_XCB_SYNC)
    check_include_files("xcb/xcb_icccm.h" FEJIX_HAS_XCB_ICCCM)
    if(NOT FEJIX_HAS_XCB_SYNC)
        message(FATAL_ERROR "Please, install libxcb-sync-dev, then clear CMake cache")
    endif()

    target_compile_definitions(fejix_runtime PUBLIC "FJ_USE_X11")

else()
    
    message(FATAL_ERROR "Platform is not supported")

endif()


# option(FEJIX_USE_OWN_RENDERER "Use Fejix's own renderer" OFF)
option(FEJIX_USE_NANOVG "Use NanoVG for rendering" ON)
option(FEJIX_USE_OPENGL3 "Use OpenGL3 for whatever renderer selected" ON)

# if(FEJIX_USE_FEJIX_OPENGL3)
#     target_compile_definitions(
#         fejix_runtime
#         PUBLIC
#         "FJ_USE_FEJIX_OPENGL3"
#         "FJ_USE_OPENGL3"
#     )
#     set(FEJIX_USE_OPENGL3 ON)

if(FEJIX_USE_NANOVG)
    target_compile_definitions(
        fejix_runtime
        PUBLIC 
        "FJ_USE_NANOVG"
    )
endif()

if(FEJIX_USE_OPENGL3)
    target_compile_definitions(
        fejix_runtime
        PUBLIC 
        "FJ_USE_OPENGL3"
    )
endif()

if(NOT FEJIX_USE_NANOVG OR NOT FEJIX_USE_OPENGL3)
    message(FATAL_ERROR "No drawing backend is selected. Plase, select one.")
endif()