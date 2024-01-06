add_rules(
    "mode.tsan", "mode.ubsan", "mode.asan", 
    "mode.debug", "mode.release"
)

add_requires(
    "fmt", 
    "gflags", 
    "gtest", 
    "concurrentqueue master",
    "benchmark", 
    "botan"
)

add_includedirs(
    "include", 
    "koios/toolpex/include", 
    "koios/include", 
    { public = true }
)

set_languages("c++2b", "c17")
set_policy("build.warning", true)
set_policy("build.optimization.lto", false)

includes("koios")

target("koioshttp")
    set_kind("shared")
    add_files("src/*.cpp")
    add_deps("koios")
    add_packages(
        "fmt", 
        "gflags", 
        "concurrentqueue", 
        "botan"
    )
    set_warnings("all", "error")
    add_cxflags("-Wconversion", { force = true })
    add_syslinks(
        "spdlog", 
        "uring"
    )
    add_files("src/*.cc")

target("test")
    set_kind("binary")
    add_packages("concurrentqueue")
    add_cxflags("-Wconversion", { force = true })
    add_deps("koioshttp", "koios")
    set_warnings("all", "error")
    add_files("test/*.cc")
    add_packages(
        "gtest", "fmt", "spdlog",
        "botan"
    )
    after_build(function (target)
        os.exec(target:targetfile())
        print("xmake: unittest complete.")
    end)
    on_run(function (target)
        --nothing
    end)
    
target("example")
    set_kind("binary")
    add_packages("")
    add_cxflags("-Wconversion", { force = true })
    add_deps("koioshttp", "koios")
    add_files( "example/*.cc")
    add_syslinks("spdlog")
    set_policy("build.warning", true)
    add_packages(
        "fmt", "gflags", 
        "concurrentqueue", 
        "botan"
    )
    

