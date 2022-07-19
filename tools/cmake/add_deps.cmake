target_link_libraries(
    fejix_runtime
    "m" # <math.h>
)


if(FEJIX_USE_X11)
    target_link_libraries(
        fejix_runtime
        "X11"
        "X11-xcb"
        "xcb"
        "xcb-sync"
        "xcb-icccm"
    )
endif()


if(FEJIX_USE_OPENGL)
    if(FEJIX_USE_OPENGL_3)
        target_include_directories(fejix_runtime PUBLIC "${ROOT}/deps/glad-opengl3.3/include")
        target_sources(fejix_runtime PRIVATE "${ROOT}/deps/glad-opengl3.3/src/gl.c")
    endif()

    if(FEJIX_USE_X11)
        if(FEJIX_USE_OPENGL_3)
            target_sources(fejix_runtime PRIVATE "${ROOT}/deps/glad-opengl3.3/src/glx.c")
        endif()
        
        target_link_libraries(fejix_runtime "dl")
    endif()

    if(FEJIX_USE_WINAPI)
        target_link_libraries(fejix_runtime "opengl32")
    endif()
endif()