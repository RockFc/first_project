
# 最低版本需求
cmake_minimum_required(VERSION 3.12)

# 工程名
set (PROJECT_NAME redis_p_p_demo)
PROJECT(${PROJECT_NAME})

# 设置option开关默认值
option(BUILD_All "Builds all projects" ON)
option(BUILD_TEST "Builds test projects" ON)

#包含系统头文件路径
include_directories(
    /usr/local/include/
    /usr/include
)
#包含系统库路径
link_directories(
    /usr/lib
    /usr/local/lib
    )

# 指定 hiredis 库的路径
set(HIREDIS_LIB /usr/lib/aarch64-linux-gnu/libhiredis.so)

# 指定 redis++ 头文件路径
set (REDIS_PLUS_PLUS_INCLUDE_DIR /workspace/git-code/redis-plus-plus/src)
include_directories(${REDIS_PLUS_PLUS_INCLUDE_DIR})
message(STATUS "redis-plus-plus header path: ${REDIS_PLUS_PLUS_INCLUDE_DIR}")

# 指定 redis++ 库的路径
set(REDIS_PLUS_PLUS_LIB /workspace/git-code/redis-plus-plus/build/libredis++.so)

# 添加头文件路径
include_directories(
    ${PROJECT_SOURCE_DIR})

#设置编译选项
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -Wall -fPIC -pipe -std=gnu90")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC -pipe -lpthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fexceptions")


#设置输出文件路径
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/product/bin)
set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/product/lib)

# <---------- set c++ standard ------------->
# NOTE: you must build redis-plus-plus and your application code with the same standard.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#打印环境信息
message("工程名: " ${PROJECT_NAME})
message("工程目录: " ${PROJECT_SOURCE_DIR})
message("编译后可执行程序目录: " ${EXECUTABLE_OUTPUT_PATH})
message("编译后库程序文件目录: " ${LIBRARY_OUTPUT_PATH})

add_subdirectory(src)