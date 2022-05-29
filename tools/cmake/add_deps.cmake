if(FEJIX_HAS_XCB)
    target_link_libraries(fejix_runtime xcb)
endif()

target_include_directories(fejix_runtime PUBLIC "${SRC}/deps/glad/include")
target_sources(fejix_runtime PRIVATE "${SRC}/deps/glad/src/glad.c")