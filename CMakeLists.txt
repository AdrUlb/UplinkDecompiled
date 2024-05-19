cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(Dependencies/tcp4u.331)

project(UplinkDecompiled)

add_executable(UplinkDecompiled
	Source/Main.cpp
	Source/App.cpp
	Source/BTree.cpp
	Source/Bungle.cpp
	Source/Eclipse.cpp
	Source/ExceptionHandling.cpp
	Source/Game.cpp
	Source/Gci.cpp
	Source/MainMenu.cpp
	Source/Network.cpp
	Source/Options.cpp
	Source/RedShirt.cpp
	Source/UplinkObject.cpp
	Source/Util.cpp
	Source/Opengl.cpp
)

target_include_directories(UplinkDecompiled PRIVATE Include)
target_compile_options(UplinkDecompiled PRIVATE -Wall -Wextra)
target_link_libraries(UplinkDecompiled tcp4u crypto SDL GL)

add_custom_target(run COMMAND $<TARGET_FILE:UplinkDecompiled> -graphics_fullscreen !graphics_screenwidth 1280 !graphics_screenheight 720)
