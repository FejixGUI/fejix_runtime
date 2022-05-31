if(FEJIX_USE_X11)
    target_link_libraries(fejix_runtime "xcb" "X11")
endif()

if(FEJIX_USE_OPENGL3)
    target_include_directories(fejix_runtime PUBLIC "${SRC}/deps/glad/include")
    target_sources(fejix_runtime PRIVATE "${SRC}/deps/glad/src/glad.c")

    if(FEJIX_USE_X11)
        target_sources(fejix_runtime PRIVATE "${SRC}/deps/glad/src/glad_glx.c")
    endif()
endif()