add_rules("mode.release-rcc", "mode.release-client")

add_requires(
    "vcpkg::curl",
    "vcpkg::curl[openssl]",
    "vcpkg::openssl",
    "vcpkg::zlib",

    -- Json library
    "vcpkg::nlohmann-json",

    -- XML library
    "vcpkg::pugixml"
)

set_optimize("fastest")
add_ldflags("/LTCG")
--set_symbols("none")

add_defines("NDEBUG")

add_defines(
    "_CRT_SECURE_NO_WARNINGS",

    "WIN32_LEAN_AND_MEAN"  -- cut down Windows.h
)

if is_mode("release-rcc") then
    add_defines("RBX_RCC")
end

-- Link user32.lib
add_links("user32")

-- Link needed windows libraries for curl
add_links("ws2_32", "wldap32", "crypt32", "normaliz", "advapi32")

-- Turn use of undefined cpp macros into errors
add_cxxflags("/we4668")

set_languages("cxx20")

includes("src")

option("with-logging")
    set_default(true)
    set_showmenu(true)
    set_description("Enable logging")

    add_defines("WITH_LOGGING")

option("standalone")
    set_default(false)
    set_showmenu(true)
    set_description("Build standalone version (no proxy on version.dll)")

    add_defines("STANDALONE")

rule("mode.release-rcc")
    on_config(function (target)
        if is_mode("release-rcc") then
            target:set("optimize", "fastest")
            target:set("strip", "all")
        end
    end)

rule("mode.release-client")
    on_config(function (target)
        if is_mode("release-client") then
            target:set("optimize", "fastest")
            target:set("strip", "all")
        end
    end)
