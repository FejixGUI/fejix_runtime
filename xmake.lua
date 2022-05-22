add_requires("libsdl", "cairo")

target("fejix_runtime")
    set_kind("static")
    add_includedirs("include", {public = true})

    -- TODO Manage multiple platform-specific files
    add_files("src/*.c")

    -- Only for development!
    -- TODO Backend selection
    add_defines("FJ_BK_USE_CAIRO", {public = true})

    -- TODO WmApi selection and detection
    add_defines("FJ_WMAPI_SDL", {public = true})

    add_packages("libsdl", {public = true})
    add_packages("cairo", {public = true})
target_end()

target("fejix_test")
    set_kind("binary")
    add_files("test/main.c")
    add_deps("fejix_runtime")
    add_packages("libsdl")
    add_packages("cairo")
target_end()