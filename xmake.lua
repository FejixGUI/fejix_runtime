target("fejix_runtime")
    set_kind("static")
    add_includedirs("include", {public = true})
    add_files("src/*.c")

    -- Only for development!
    add_defines("FJ_BK_USE_CAIRO", {public = true})
target_end()

target("fejix_test")
    set_kind("binary")
    add_files("test/main.c")
    add_deps("fejix_runtime")
target_end()