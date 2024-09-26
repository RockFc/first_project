
# 最低版本需求
cmake_minimum_required(VERSION 3.15)

# 工程名
set (PROJECT_NAME uWebSockets_demo)
PROJECT(${PROJECT_NAME})

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# 设置option开关默认值
option(BUILD_All "Builds all projects" ON)
option(BUILD_TEST "Builds test projects" ON)

# 链接 OpenSSL 和 Zlib (如果需要)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)

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

# 指定 uSockets 头文件路径
set (LIBUSOCKETS_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/../../uWebSockets/uSockets/src)
# 指定 uSockets 库的路径 (uWebSockets头文件库依赖 usockets 库)
set(LIBUSOCKETS_LIB ${PROJECT_SOURCE_DIR}/../../uWebSockets/uSockets/uSockets.a)
# 指定 uWebSockets 头文件路径
set (LIBUWEBSOCKETS_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/../../uWebSockets/src)
# 指定 nlohmann 头文件路径
set (NLOHMANN_JSON_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/../../json/include)

# 添加头文件路径
include_directories(
    ${PROJECT_SOURCE_DIR}
    ${LIBUSOCKETS_INCLUDE_DIR}
    ${LIBUWEBSOCKETS_INCLUDE_DIR}
    ${NLOHMANN_JSON_INCLUDE_DIR}
    )

#设置输出文件路径
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/product/bin)
set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/product/lib)

#打印环境信息
message("工程名: " ${PROJECT_NAME})
message("工程目录: " ${PROJECT_SOURCE_DIR})
message("编译后可执行程序目录: " ${EXECUTABLE_OUTPUT_PATH})
message("编译后库程序文件目录: " ${LIBRARY_OUTPUT_PATH})

add_subdirectory(src)