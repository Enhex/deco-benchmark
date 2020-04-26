newoption {
	trigger     = "location",
	value       = "./",
	description = "Where to generate the project.",
}

if not _OPTIONS["location"] then
	_OPTIONS["location"] = "./"
end

include(_OPTIONS["location"] .. "conanbuildinfo.premake.lua")

workspace("benchmark")
	location(_OPTIONS["location"])
	conan_basic_setup()

	project("benchmark")
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"
		targetdir = _OPTIONS["location"] .. "bin/%{cfg.buildcfg}"

		defines{"_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS"}

		files{
			"src/*",
		}

		filter "configurations:Debug"
			defines { "DEBUG" }
			symbols "On"

		filter "configurations:Release"
			defines { "NDEBUG" }
			optimize "On"
            flags {"LinkTimeOptimization"}
