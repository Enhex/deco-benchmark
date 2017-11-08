if (MSVC)
	cmake_minimum_required(VERSION 3.10)	# to be able to set C++17
else()
	cmake_minimum_required(VERSION 3.8)		# to be able to set C++17
endif(MSVC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo_multi.cmake)

project(benchmark)
conan_basic_setup()

add_executable(benchmark benchmark.cpp)
conan_target_link_libraries(benchmark)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT benchmark)
if (MSVC)
	set_target_properties(benchmark PROPERTIES COMPILE_FLAGS "/GL")
	set_target_properties(benchmark PROPERTIES LINK_FLAGS "/LTCG")
endif(MSVC)