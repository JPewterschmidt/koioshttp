add_rules("mode.debug", "mode.release")

includes("koios", "toolpex")

add_requires(
    "gtest"
)

set_languages("c++23", "c17")
set_policy("build.warning", true)
set_policy("build.optimization.lto", false)
set_toolset("cc", "mold", {force = true}) 

if not is_mode("release") then
    add_defines("KOIOS_DEBUG", {force = true})
end

if is_mode("asan") then
    add_cxxflags("-fno-inline", {force = true})
    set_optimize("none", {force = true})
end

if is_mode("release") then
    set_optimize("fastest", {force = true})
end

target("koioshttp")
    set_kind("shared")
    add_deps("koios")
    add_files("src/*.cc")
    set_policy("build.warning", true)
    add_cxflags("-Wconversion", { force = true })
    add_packages(
        "uriparser", 
        "llhttp", 
        "spdlog"
    )
    add_includedirs(
        "include", 
        { public = true }
    )
