cmake_minimum_required(VERSION 3.16)

if(NOT DEFINED ENV{UPLINK_DIR})
    message(FATAL_ERROR "The environment variable UPLINK_DIR must be set to the Uplink game directory.")
endif()

project(UplinkDecompiled)

add_library(UplinkDecompiled SHARED UplinkDecompiled.cpp Util.cpp)
target_compile_options(UplinkDecompiled PRIVATE -m32)
target_link_options(UplinkDecompiled PRIVATE -m32)

add_custom_target(run COMMAND LD_PRELOAD=$<TARGET_FILE:UplinkDecompiled> '$ENV{UPLINK_DIR}/uplink.bin.x86')
