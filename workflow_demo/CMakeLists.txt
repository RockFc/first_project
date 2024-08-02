
# 最低版本需求
cmake_minimum_required(VERSION 3.12)

# 工程名
set (PROJECT_NAME http_svr_demo)
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

# 添加头文件路径
include_directories(
    ${PROJECT_SOURCE_DIR})

#设置编译选项
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -Wall -fPIC -pipe -std=gnu90")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC -pipe -lpthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fexceptions")

if(ANDROID)
	link_directories(${OPENSSL_LINK_DIR})
else()
	find_library(LIBRT rt)
	find_package(OpenSSL REQUIRED)
endif()

find_package(workflow REQUIRED CONFIG HINTS ..)
include_directories(${OPENSSL_INCLUDE_DIR} ${WORKFLOW_INCLUDE_DIR})
link_directories(${WORKFLOW_LIB_DIR})

if (APPLE)
	set(WORKFLOW_LIB workflow pthread OpenSSL::SSL OpenSSL::Crypto)
elseif (ANDROID)
	set(WORKFLOW_LIB workflow ssl crypto c)
else ()
	set(WORKFLOW_LIB workflow pthread OpenSSL::SSL OpenSSL::Crypto ${LIBRT})
endif ()

#设置输出文件路径
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/product/bin)
set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/product/lib)

# <---------- set c++ standard ------------->
# NOTE: you must build redis-plus-plus and your application code with the same standard.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#打印环境信息
message("工程名: " ${PROJECT_NAME})
message("工程目录: " ${PROJECT_SOURCE_DIR})
message("编译后可执行程序目录: " ${EXECUTABLE_OUTPUT_PATH})
message("编译后库程序文件目录: " ${LIBRARY_OUTPUT_PATH})
message("WORKFLOW_INCLUDE_DIR: " ${WORKFLOW_INCLUDE_DIR})
message("WORKFLOW_LIB_DIR: " ${WORKFLOW_LIB_DIR})

add_subdirectory(src)