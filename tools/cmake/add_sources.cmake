target_sources(fejix_runtime
    PRIVATE
    "${ROOT}/src/generic.c"
    "${ROOT}/src/drawing.c"
    "${ROOT}/src/layout.c"
    "${ROOT}/src/stdlayout.c"
    "${ROOT}/src/backend/backend_init.c"
)


if(FEJIX_USE_NANOVG)
    target_sources(
        fejix_runtime PRIVATE
        "${ROOT}/src/backend/nanovg/nanovg.c"
        "${ROOT}/src/backend/nanovg/drawing.c"
    )
endif()


if(FEJIX_USE_WINAPI)

    target_sources(
        fejix_runtime PRIVATE
        "${ROOT}/src/platform/mswindows/winapi.c"
    )

    if(FEJIX_USE_OPENGL3)
        target_sources(
            fejix_runtime PRIVATE
            "${ROOT}/src/platform/mswindows/opengl3/opengl3.c"
        )
    endif()
endif()


if(FEJIX_USE_X11)

    target_sources(
        fejix_runtime PRIVATE
        "${ROOT}/src/platform/x11/x11.c"
        "${ROOT}/src/platform/x11/window_utils.c"
        "${ROOT}/src/platform/x11/main_loop.c"
    )

    if(FEJIX_USE_OPENGL3)
        target_sources(
            fejix_runtime PRIVATE
            "${ROOT}/src/platform/x11/opengl3/opengl3.c"
        )
    endif()

endif()