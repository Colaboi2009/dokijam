cmake_minimum_required(VERSION 3.30)
project(doki_jam)

file(GLOB_RECURSE sources src/*.cpp include/*.cpp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} ${sources})

target_compile_options(${PROJECT_NAME} PUBLIC
	-std=c++23
)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} SDL3 SDL3_image SDL3_ttf z)
target_include_directories(${PROJECT_NAME} PUBLIC 
	src
	include
)

