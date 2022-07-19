target_include_directories(fejix_runtime PUBLIC "include")

target_sources(fejix_runtime PRIVATE
    "${ROOT}/src/generic.c"
)


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
        "${ROOT}/src/platform/x11/main_loop.c"
        "${ROOT}/src/platform/x11/utils.c"
    )

    if(FEJIX_USE_OPENGL)
        target_sources(
            fejix_runtime PRIVATE
            "${ROOT}/src/platform/x11/opengl/opengl.c"
        )
    endif()

endif()