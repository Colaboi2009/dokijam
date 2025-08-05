cmake_minimum_required(VERSION 3.27)
project(doki_jam)

SET(CMAKE_CXX_STANDARD 23)
SET(CMAKE_C_STANDARD 17)

file(GLOB_RECURSE sources src/*.cpp include/*.cpp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} ${sources})

set(SDL_HAPTIC OFF CACHE BOOL "" FORCE)
set(SDL_JOYSTICK OFF CACHE BOOL "" FORCE)

add_subdirectory("vendor/zlib")
add_subdirectory("vendor/SDL")
add_subdirectory("vendor/SDL_image")
add_subdirectory("vendor/SDL_ttf")
add_subdirectory("vendor/entt")

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
	SDL3::SDL3
	SDL3_image::SDL3_image
	SDL3_ttf::SDL3_ttf
	ZLIB::ZLIBSTATIC
    EnTT::EnTT
)
target_include_directories(${PROJECT_NAME} PUBLIC 
	src
	include
)

function(copy_dll target source)
    add_custom_command(
        TARGET ${target} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:${source}>" "$<TARGET_FILE_DIR:${target}>"
        VERBATIM
    )
endfunction()

if (WIN32)
    copy_dll(doki_jam SDL3::SDL3)
    copy_dll(doki_jam SDL3_image::SDL3_image)
    copy_dll(doki_jam SDL3_ttf::SDL3_ttf)
endif()

add_custom_command(TARGET doki_jam POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/art/"
        "$<TARGET_FILE_DIR:doki_jam>/art"
)
