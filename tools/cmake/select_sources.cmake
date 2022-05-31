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
        "${SRC}/src/platform/x11/fejix_xcb.c"
    )

endif()