cmake_minimum_required(VERSION 3.16)

if(NOT DEFINED ENV{UPLINK_DIR})
    message(FATAL_ERROR "The environment variable UPLINK_DIR must be set to the Uplink game directory.")
endif()

project(UplinkDecompiled)

set(UPLINK_SOURCES Source/Uplink/Util.cpp Source/Uplink/Graphics.cpp Source/Uplink/Sound.cpp Source/Uplink/Network.cpp Source/Uplink/Game.cpp Source/Uplink/MainMenu.cpp Source/Uplink/Main.cpp Source/Uplink/RedShirt.cpp Source/Uplink/Options.cpp Source/Uplink/Bungle.cpp Source/Uplink/App.cpp Source/Uplink/UplinkObject.cpp ../unrar/sha1.cpp)
add_library(UplinkDecompiled SHARED Source/UplinkDecompiled.cpp ${UPLINK_SOURCES})
target_compile_options(UplinkDecompiled PRIVATE -m32 -Wall -Wextra)
target_link_options(UplinkDecompiled PRIVATE -m32)

add_custom_target(run
    COMMAND LD_PRELOAD=$<TARGET_FILE:UplinkDecompiled> '$ENV{UPLINK_DIR}/uplink.bin.x86')
