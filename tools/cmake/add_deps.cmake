target_include_directories(fejix_runtime PUBLIC "${SRC}/deps/glad/include")
target_sources(fejix_runtime PRIVATE "${SRC}/deps/glad/src/glad.c")