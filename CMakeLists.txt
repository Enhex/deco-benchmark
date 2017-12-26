if (MSVC)
	cmake_minimum_required(VERSION 3.10)	# to be able to set C++17
else()
	cmake_minimum_required(VERSION 3.8)		# to be able to set C++17
endif(MSVC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)

project(benchmark)
conan_basic_setup()

file(GLOB_RECURSE SourceFiles
    "src/*.cpp"
    "src/*.h"
    "src/*.hpp"
)


set(TargetName benchmark)
add_executable(${TargetName} ${SourceFiles})
conan_target_link_libraries(${TargetName})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TargetName})
if (MSVC)
	set_target_properties(${TargetName} PROPERTIES COMPILE_FLAGS "/GL")
	set_target_properties(${TargetName} PROPERTIES LINK_FLAGS "/LTCG")
endif(MSVC)