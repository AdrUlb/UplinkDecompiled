cmake_minimum_required(VERSION 3.28)

project(tcp4u)

set(HTTP4U_SRCS src/http4u.c src/htp4_url.c src/htp4_err.c src/util.c)
set(TCP4U_SRCS src/tcp4u.c src/tcp4_err.c src/tcp4_log.c src/tcp4u_ex.c src/skt4u.c src/tn4u.c src/tn_proto.c)
set(UDP4U_SRCS src/udp4u.c)
set(SMTP4U_SRCS src/smtp4u.c)
set(SRCS ${HTTP4U_SRCS} ${TCP4U_SRCS} ${UDP4U_SRCS} ${SMTP4U_SRCS})

add_library(tcp4u SHARED ${SRCS})
target_include_directories(tcp4u PUBLIC Include)
target_compile_options(tcp4u PRIVATE -DUNIX -D${CMAKE_SYSTEM_NAME})
