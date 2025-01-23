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
    add_files("src/*.cc")
    set_policy("build.warning", true)
    add_cxflags("-Wconversion", { force = true })
    add_includedirs(
        "include", 
        { public = true }
    )

target("koioshttp-example")
    set_kind("binary")
    add_files("example/*.cc")
    add_deps("koioshttp")

target("koioshttp-test")
    set_kind("binary")
    add_packages(
        "gtest"
    )
    add_files("test/*.cc")
    after_build(function (target)
        os.execv(target:targetfile(), {"--gtest_color=yes"})
        print("xmake: unittest complete.")
    end)
    on_run(function (target)
        --nothing
    end)
