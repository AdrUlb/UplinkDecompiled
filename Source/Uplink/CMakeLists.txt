cmake_minimum_required(VERSION 3.16)

if(NOT DEFINED ENV{UPLINK_DIR})
    message(FATAL_ERROR "The environment variable UPLINK_DIR must be set to the Uplink game directory.")
endif()

project(UplinkDecompiled)

set(UPLINK_SOURCES
    Source/App.cpp
    Source/Bungle.cpp
    Source/Ecl.cpp
    Source/FrameRate.cpp
    Source/Game.cpp
    Source/Graphics.cpp
    Source/Main.cpp
    Source/MainMenu.cpp
    Source/Network.cpp
    Source/Options.cpp
    Source/RedShirt.cpp
    Source/Sound.cpp
    Source/UplinkObject.cpp
    Source/Util.cpp
    ../../unrar/sha1.cpp)

add_library(UplinkDecompiled SHARED UplinkDecompiled.cpp ${UPLINK_SOURCES})
target_include_directories(UplinkDecompiled PRIVATE Include)
target_compile_options(UplinkDecompiled PRIVATE -m32 -Wall -Wextra -DUPLINKDECOMP32=1)
target_link_options(UplinkDecompiled PRIVATE -m32)

add_library(UplinkDecompiled64 SHARED UplinkDecompiled.cpp ${UPLINK_SOURCES})
target_include_directories(UplinkDecompiled64 PRIVATE Include)
target_compile_options(UplinkDecompiled64 PRIVATE -Wall -Wextra -DUPLINKDECOMP64=1)
target_link_options(UplinkDecompiled64 PRIVATE)

add_custom_target(run
    COMMAND LD_PRELOAD=$<TARGET_FILE:UplinkDecompiled> '$ENV{UPLINK_DIR}/uplink.bin.x86')

 add_custom_target(run64
COMMAND LD_PRELOAD=$<TARGET_FILE:UplinkDecompiled64> '$ENV{UPLINK_DIR}/uplink.bin.x86_64')
