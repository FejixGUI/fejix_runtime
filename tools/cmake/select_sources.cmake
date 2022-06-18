option(FEJIX_USE_STDUI "Build standard UI utilities" ON)

target_sources(fejix_runtime
    PRIVATE
    "${SRC}/src/fejix_generic.c"
    "${SRC}/src/fejix_layout.c"
)

if(FEJIX_USE_STDUI)
    target_sources(fejix_runtime
        PRIVATE
        "${SRC}/src/fejix_stdui.c"
    )
endif()

target_include_directories(
    fejix_runtime PRIVATE
    "${SRC}/src"
)

if (FEJIX_USE_NANOVG)
    target_sources(
        fejix_runtime PRIVATE
        "${SRC}/src/fejix_drawer/nanovg/nanovg.c"
    )
endif()

if(FEJIX_USE_WINAPI)

    target_sources(
        fejix_runtime PRIVATE
        "${SRC}/src/fejix_platform/mswindows/fejix_winapi.c"
    )

    if(FEJIX_USE_OPENGL3)
        target_sources(
            fejix_runtime PRIVATE
            "${SRC}/src/fejix_platform/mswindows/opengl3/opengl3.c"
        )
    endif()

elseif(FEJIX_USE_X11)

    target_sources(
        fejix_runtime PRIVATE
        "${SRC}/src/fejix_platform/x11/x11.c"
        "${SRC}/src/fejix_platform/x11/window_utils.c"
        "${SRC}/src/fejix_platform/x11/main_loop.c"
    )

    if(FEJIX_USE_OPENGL3)
        target_sources(
            fejix_runtime PRIVATE
            "${SRC}/src/fejix_platform/x11/opengl3/opengl3.c"
        )
    endif()

endif()