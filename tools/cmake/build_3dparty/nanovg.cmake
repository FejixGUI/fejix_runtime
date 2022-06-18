target_sources(
    fejix_runtime
    PRIVATE
    "${SRC}/deps/nanovg/src/nanovg.c"
)

target_include_directories(
    fejix_runtime
    PRIVATE
    "${SRC}/deps/nanovg/src"
)