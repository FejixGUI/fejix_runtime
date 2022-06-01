target_sources(fejix_runtime
    PRIVATE
    "${SRC}/src/fejix_generic.c"
    # "${SRC}/src/fejix_stdui.c"
)

if(FEJIX_USE_WINAPI)

    target_sources(
        fejix_runtime PRIVATE
        "${SRC}/src/platform/mswindows/fejix_winapi.c"
    )

elseif(FEJIX_USE_X11)

    target_sources(
        fejix_runtime PRIVATE
        "${SRC}/src/platform/x11/fejix_x11.c"
        "${SRC}/src/platform/x11/fejix_backends.c"
    )

    target_include_directories(
        fejix_runtime PRIVATE
        "${SRC}/src/platform/x11"
    )

    if(FEJIX_USE_OPENGL3)
        target_sources(
            fejix_runtime PRIVATE
            "${SRC}/src/platform/x11/opengl3/opengl3.c"
        )
    endif()

endif()