cmake_minimum_required(VERSION 3.10)
project(my-lwip-app)
set (LWIP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../lwip)
set (LWIP_INCLUDE_DIRS
    "${LWIP_DIR}/src/include"
    "${LWIP_DIR}/contrib"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)
include(${LWIP_DIR}/contrib/ports/CMakeCommon.cmake)
include(${LWIP_DIR}/src/Filelists.cmake)
include(${LWIP_DIR}/contrib/Filelists.cmake)
#include(${LWIP_DIR}/contrib/ports/unix/Filelists.cmake)
add_executable(mylwip main.c)
target_include_directories(mylwip PRIVATE ${LWIP_INCLUDE_DIRS})
target_link_libraries(mylwip lwipcore)
