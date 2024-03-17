cmake_minimum_required(VERSION 3.28)

project(UplinkDecompiled)

add_executable(UplinkDecompiled
	Source/Main.cpp
	Source/App.cpp
	Source/Options.cpp
	Source/UplinkObject.cpp
	Source/Util.cpp
)
target_include_directories(UplinkDecompiled PRIVATE Include)
target_compile_options(UplinkDecompiled PRIVATE -Wall -Wextra)
