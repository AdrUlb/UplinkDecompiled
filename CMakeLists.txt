cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-DNDEBUG")

add_subdirectory(Dependencies/tcp4u.331)
add_subdirectory(Dependencies/ftgl-2.4.0)


project(UplinkDecompiled)

file(GLOB_RECURSE UPLINK_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp)

add_executable(UplinkDecompiled ${UPLINK_SOURCES})

target_include_directories(UplinkDecompiled PRIVATE Include)
target_compile_options(UplinkDecompiled PRIVATE -Wall -Wextra)
target_link_libraries(UplinkDecompiled tcp4u crypto SDL GL ftgl SDL_mixer)

add_custom_target(run COMMAND $<TARGET_FILE:UplinkDecompiled> -graphics_fullscreen !graphics_screenwidth 1280 !graphics_screenheight 720)
