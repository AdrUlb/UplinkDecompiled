cmake_minimum_required(VERSION 3.28)

add_subdirectory(Dependencies/tcp4u.331)

project(UplinkDecompiled)

add_executable(UplinkDecompiled
	Source/Main.cpp
	Source/App.cpp
	Source/BTree.cpp
	Source/Bungle.cpp
	Source/Ecl.cpp
	Source/Gci.cpp
	Source/MainMenu.cpp
	Source/Network.cpp
	Source/Options.cpp
	Source/RedShirt.cpp
	Source/UplinkObject.cpp
	Source/Util.cpp
)

target_include_directories(UplinkDecompiled PRIVATE Include)
target_compile_options(UplinkDecompiled PRIVATE -Wall -Wextra)
target_link_libraries(UplinkDecompiled tcp4u)
target_link_libraries(UplinkDecompiled -lcrypto)
