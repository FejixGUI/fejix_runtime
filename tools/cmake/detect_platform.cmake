# Let's make it clear.
# FEJIX_FORCE_XXX - argument for this script; forces to use XXX
# FEJIX_USE_XXX - result of this script that indicated that XXX will be used
# FEJIX_HAS_XXX - a header XXX.h is detected

check_include_files("windows.h" FEJIX_HAS_WINAPI)
# check_include_files("xcb/xcb.h" FEJIX_HAS_XCB)
# check_include_files("X11/Xlib.h" FEJIX_HAS_XLIB)
check_include_files("X11/Xlib-xcb.h" FEJIX_HAS_X11)

set(FEJIX_USE_WINAPI OFF)
set(FEJIX_USE_X11 OFF)

if(FEJIX_HAS_WINAPI)

    set(FEJIX_USE_WINAPI ON)

    target_compile_definitions(fejix_runtime PUBLIC "FJ_USE_WINAPI")

elseif(FEJIX_HAS_X11)

    set(FEJIX_USE_X11 ON)

    target_compile_definitions(fejix_runtime PUBLIC "FJ_USE_X11")

else()
    
    message(FATAL_ERROR "Platform is not supported")

endif()


option(FEJIX_USE_OPENGL3 "Use OpenGL 3" ON)

if(FEJIX_USE_OPENGL3)
    target_compile_definitions(fejix_runtime PUBLIC "FJ_USE_OPENGL3")
else()
    message(FATAL_ERROR "No drawing backend is selected. Plase, select one.")
endif()