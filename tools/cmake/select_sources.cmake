target_sources(fejix_runtime
    PRIVATE
    "${SRC}/src/platform/fejix_backend_wrapper.c"
    # "${SRC}/src/fejix_stdui.c"
)

target_include_directories(
    fejix_runtime PRIVATE
    "${SRC}/src/platform"
)

if(FEJIX_USE_WINAPI)

    target_sources(
        fejix_runtime PRIVATE
        "${SRC}/src/platform/mswindows/fejix_winapi.c"
    )

    target_include_directories(
        fejix_runtime PRIVATE
        "${SRC}/platform/mswindows"
    )

    if(FEJIX_USE_OPENGL3)
        target_sources(
            fejix_runtime PRIVATE
            "${SRC}/src/platform/mswindows/opengl3/opengl3.c"
        )
    endif()

elseif(FEJIX_USE_X11)

    target_sources(
        fejix_runtime PRIVATE
        "${SRC}/src/platform/x11/fejix_x11.c"
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