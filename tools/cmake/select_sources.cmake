target_sources(fejix_runtime
    PRIVATE
    "${SRC}/fejix_generic.c"
    "${SRC}/fejix_stdui.c"
)

if(FEJIX_HAS_WINAPI)

    target_sources(
        fejix_runtime PRIVATE
        "${SRC}/platform/winapi/fejix_winapi.c"
    )

elseif(FEJIX_HAS_XLIB)

    target_sources(
        fejix_runtime PRIVATE
        "${SRC}/platform/xlib/fejix_xlib.c"
    )

endif()