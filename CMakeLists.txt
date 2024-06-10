cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-DNDEBUG -O3")
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-g -rdynamic")

add_subdirectory(Dependencies/tcp4u.331)
add_subdirectory(Dependencies/ftgl-2.4.0)

project(UplinkDecompiled CXX)

find_package(Freetype REQUIRED)
find_package(SDL REQUIRED)
find_package(SDL_mixer REQUIRED)
find_package(TIFF REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenSSL REQUIRED)

file(GLOB_RECURSE UPLINK_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp)

add_executable(UplinkDecompiled ${UPLINK_SOURCES})

target_include_directories(UplinkDecompiled PRIVATE Include ${SDL_INCLUDE_DIRS} ${SDL_MIXER_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS} ${TIFF_INCLUDE_DIRS})
target_compile_options(UplinkDecompiled PRIVATE -Wall -Wextra)
target_link_libraries(UplinkDecompiled ftgl tcp4u ${SDL_LIBRARIES} ${SDL_MIXER_LIBRARIES} ${OPENGL_LIBRARIES} ${OPENSSL_CRYPTO_LIBRARY} ${FREETYPE_LIBRARIES} ${TIFF_LIBRARIES})

add_custom_target(run COMMAND $<TARGET_FILE:UplinkDecompiled> -game_firsttime -graphics_fullscreen +graphics_fasterbuttonanimations !graphics_screenwidth 640 !graphics_screenheight 480)
