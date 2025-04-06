-- set minimum xmake version
set_xmakever("2.8.2")

-- includes
includes("lib/commonlibsse-ng")

-- set project
set_project("stages-of-suffering")
set_version("1.0.0")
set_license("GPL-3.0")

-- set defaults
set_languages("c++23")
set_warnings("allextra")

-- set policies
set_policy("package.requires_lock", true)
set_config("rex_ini", true)



set_config("rex_json", true)
add_requires("nlohmann_json")
-- add rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- extra files
add_extrafiles("release/**.ini")

-- targets
target("stages-of-suffering")
    -- add dependencies to target
    add_deps("commonlibsse-ng")
    add_packages("nlohmann_json")

    -- add commonlibsse-ng plugin
    add_rules("commonlibsse-ng.plugin", {
        name = "stages-of-suffering",
        author = "styyx",
        description = "Injury based death alternative mod"
    })

    -- add src files
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")

after_build(function(target)
    local copy = function(env, ext)
        for _, env in pairs(env:split(";")) do
            if os.exists(env) then
                local plugins = path.join(env, ext, "SKSE/Plugins")
                os.mkdir(plugins)
                os.trycp(target:targetfile(), plugins)
                os.trycp(target:symbolfile(), plugins)
                os.trycp("$(projectdir)/release/**.ini", plugins)
            end
        end
    end
    if os.getenv("XSE_TES5_MODS_PATH") then
        copy(os.getenv("XSE_TES5_MODS_PATH"), target:name())
    elseif os.getenv("XSE_TES5_GAME_PATH") then
        copy(os.getenv("XSE_TES5_GAME_PATH"), "Data")
    end
end)