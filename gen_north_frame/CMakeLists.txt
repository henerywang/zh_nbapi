cmake_minimum_required(VERSION 3.10) # 指定CMake的最小版本要求

# 项目名称
project(genNbFrame VERSION 1.0)

# 指定C标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")


file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

# 指定头文件目录，这样在编译时可以找到头文件
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}  # 假设头文件和源文件在同一目录下
    /home/henery/switch-daemons/ops-intfd-master/src/north_yang_lib/include
)
foreach(file IN LISTS SOURCES)
    if(file MATCHES "${CMAKE_CURRENT_SOURCE_DIR}/build/.*\\.c$")
        list(REMOVE_ITEM SOURCES ${file})
    endif()
endforeach()
message(STATUS "Updated source files: ${SOURCES}")

# 添加宏定义

add_executable(genNbFrame ${SOURCES})

# 链接 libfrr 库
# 假设 libfrr.so 位于 /usr/local/lib 目录下
# 链接目标和库
#target_link_libraries(jzEthers PRIVATE frr)
target_link_libraries(genNbFrame /home/henery/switch-daemons/ops-intfd-master/src/north_yang_lib/build/libyangnb.so)
target_link_libraries(genNbFrame /home/henery/libyang-3.4.2/build/libyang.so)
target_link_libraries(genNbFrame libjson-c.so)
