cmake_minimum_required(VERSION 3.10)  # 指定CMake的最小版本要求

# 项目名称
project(YangNB)

# 指定C语言标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

# 设置 Python 3 可执行文件路径
find_package(Python3 REQUIRED)
set(PYTHON3_EXECUTABLE ${Python3_EXECUTABLE})

# 获取 embedmodel.py 的绝对路径
set(EMBEDMODEL_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/yang_module/embedmodel.py")

# 递归获取 yang_module 目录及其子目录下的所有 .yang 文件
file(GLOB_RECURSE YANG_FILES "${CMAKE_CURRENT_SOURCE_DIR}/yang_module/*.yang")
message(STATUS "Updated source yang files: ${YANG_FILES}")
# 用于存储生成的 .c 文件的列表
set(GENERATED_C_FILES "")

# 为每个 .yang 文件生成一个自定义命令和目标
foreach(YANG_FILE ${YANG_FILES})
    get_filename_component(YANG_FILE_NAME ${YANG_FILE} NAME_WE)
    get_filename_component(YANG_FILE_DIR ${YANG_FILE} DIRECTORY)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" YANG_FILE_DIR_RELATIVE ${YANG_FILE_DIR})
    set(C_FILE "${YANG_FILE_NAME}.c")
    
    # 确保生成目录存在
    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/${YANG_FILE_DIR_RELATIVE})
    
    # 添加自定义命令
    add_custom_command(
        OUTPUT ${C_FILE}
        COMMAND ${PYTHON3_EXECUTABLE} ${EMBEDMODEL_SCRIPT} ${YANG_FILE_NAME}.yang ${C_FILE}
        WORKING_DIRECTORY ${YANG_FILE_DIR}  # 指定工作目录
        DEPENDS ${YANG_FILE}
        VERBATIM
    )
    
    # 添加生成的 .c 文件到列表
    list(APPEND GENERATED_C_FILES ${C_FILE})
endforeach()

# 添加一个自定义目标来确保所有生成的 .c 文件都存在
add_custom_target(generate_c_files ALL DEPENDS ${GENERATED_C_FILES})
# 指定要编译的源文件

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.c) 

# 指定头文件目录，这样在编译时可以找到头文件
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include  # 假设头文件和源文件在同一目录下
)
foreach(file IN LISTS SOURCES)
    if(file MATCHES "${CMAKE_CURRENT_SOURCE_DIR}/build/.*\\.c$")
        list(REMOVE_ITEM SOURCES ${file})
    endif()
endforeach()
message(STATUS "Updated source files: ${SOURCES}")
# 添加动态库
add_library(yangnb SHARED ${SOURCES})

# 链接依赖的动态库
target_link_libraries(yangnb /home/henery/libyang-3.4.2/build/libyang.so)

# 设置输出的库文件名称
set_target_properties(yangnb PROPERTIES
    OUTPUT_NAME "yangnb"
    PREFIX "lib"
    SUFFIX ".so"
)

