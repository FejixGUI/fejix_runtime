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

if(FEJIX_USE_OPENGL3)
    target_include_directories(fejix_runtime PUBLIC "${ROOT}/deps/glad/include")
    target_sources(fejix_runtime PRIVATE "${ROOT}/deps/glad/src/gl.c")

    if(FEJIX_USE_X11)
        target_sources(fejix_runtime PRIVATE "${ROOT}/deps/glad/src/glx.c")
        target_link_libraries(fejix_runtime "dl")
    endif()

    if(FEJIX_USE_WINAPI)
        target_link_libraries(fejix_runtime "opengl32")
    endif()
endif()

if(FEJIX_USE_NANOVG)
    target_sources(fejix_runtime PRIVATE "${ROOT}/deps/nanovg/src/nanovg.c")
    target_include_directories(fejix_runtime PRIVATE "${ROOT}/deps/nanovg/src")
endif()