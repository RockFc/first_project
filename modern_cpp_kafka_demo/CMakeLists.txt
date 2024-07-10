
# 最低版本需求
cmake_minimum_required(VERSION 3.12)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

set (PROJECT_NAME cpp_kafka_demo)
PROJECT(${PROJECT_NAME})

# 查找依赖
# 查找 rdkafka 库
#---------------------------
# Check Dependencies
#---------------------------

#---------------------------
# librdkafka library
#---------------------------
if (DEFINED ENV{LIBRDKAFKA_INCLUDE_DIR})
    set(LIBRDKAFKA_INCLUDE_DIR $ENV{LIBRDKAFKA_INCLUDE_DIR})
else ()
    find_file(LIBRDKAFKA_HEADER
        NAMES rdkafka.h
        HINTS /usr/include/librdkafka /usr/local/include/librdkafka /opt/homebrew/include/librdkafka)

    cmake_path(GET LIBRDKAFKA_HEADER PARENT_PATH LIBRDKAFKA_INCLUDE_DIR)
    cmake_path(GET LIBRDKAFKA_INCLUDE_DIR PARENT_PATH LIBRDKAFKA_INCLUDE_DIR)
endif ()

if (DEFINED ENV{LIBRDKAFKA_LIBRARY_DIR})
    set(LIBRDKAFKA_LIBRARY_DIR $ENV{LIBRDKAFKA_LIBRARY_DIR})
else ()
    find_library(LIBRDKAFKA_LIB
        NAMES rdkafka
        HINTS /usr/lib /usr/local/lib /opt/homebrew/lib)

    cmake_path(GET LIBRDKAFKA_LIB PARENT_PATH LIBRDKAFKA_LIBRARY_DIR)
endif ()

if (EXISTS "${LIBRDKAFKA_INCLUDE_DIR}/librdkafka/rdkafka.h")
    message(STATUS "librdkafka include directory: ${LIBRDKAFKA_INCLUDE_DIR}")
else ()
    message(FATAL_ERROR "Could not find headers for librdkafka!")
endif ()

if (EXISTS "${LIBRDKAFKA_LIBRARY_DIR}/librdkafka.a" OR EXISTS "${LIBRDKAFKA_LIBRARY_DIR}/librdkafka.so" OR EXISTS "${LIBRDKAFKA_LIBRARY_DIR}/rdkafka.lib" )
    message(STATUS "librdkafka library directory: ${LIBRDKAFKA_LIBRARY_DIR}")
else ()
    message(FATAL_ERROR "Could not find library for librdkafka!")
endif ()


# 包含 rdkafka 头文件路径
include_directories(${LIBRDKAFKA_INCLUDE_DIR}/librdkafka)
message(STATUS "LIBRDKAFKA_INCLUDE_DIR path: ${LIBRDKAFKA_INCLUDE_DIR}")

# 指定 rdkafka 库的路径
set(LIBRDKAFKA_LIBRARY_LIB ${LIBRDKAFKA_LIBRARY_DIR}/librdkafka.so)
message(STATUS "LIBRDKAFKA_LIBRARY_LIB path: ${LIBRDKAFKA_LIBRARY_LIB}")

# 包含 modern-cpp-kafka 路径
include_directories(${PROJECT_SOURCE_DIR}/../../modern-cpp-kafka/include)

#设置输出文件路径
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/product/bin)
set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/product/lib)

#打印环境信息
message("工程名: " ${PROJECT_NAME})
message("工程目录: " ${PROJECT_SOURCE_DIR})
message("编译后可执行程序目录: " ${EXECUTABLE_OUTPUT_PATH})
message("编译后库程序文件目录: " ${LIBRARY_OUTPUT_PATH})

add_subdirectory(src)