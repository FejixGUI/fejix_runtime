if(FEJIX_USE_X11)
    target_link_libraries(fejix_runtime "xcb" "X11" "X11-xcb")
endif()

if(FEJIX_USE_OPENGL3)
    target_include_directories(fejix_runtime PUBLIC "${SRC}/deps/glad/include")
    target_sources(fejix_runtime PRIVATE "${SRC}/deps/glad/src/gl.c")

    if(FEJIX_USE_X11)
        target_sources(fejix_runtime PRIVATE "${SRC}/deps/glad/src/glx.c")
        target_link_libraries(fejix_runtime "dl")
    endif()

    if(FEJIX_USE_WINAPI)
        target_link_libraries(fejix_runtime "opengl32")
    endif()
endif()