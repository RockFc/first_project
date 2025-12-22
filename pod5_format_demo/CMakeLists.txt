cmake_minimum_required(VERSION 3.12)
project(pod5_format_demo)

# 1. 基础编译选项
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# 2. 定义 Thirdparty 路径变量
set(TP_ROOT "${PROJECT_SOURCE_DIR}/thirdparty/pod5")
set(TP_INCLUDE_DIR "${TP_ROOT}/include")
set(TP_LIB_DIR     "${TP_ROOT}/lib")

# 显式指定库文件路径变量，供子目录使用
set(POD5_LIB        "${TP_LIB_DIR}/libpod5_format.a")
set(ARROW_LIB       "${TP_LIB_DIR}/libarrow.so")
set(CURL_LIB        "${TP_LIB_DIR}/libcurl.so.4")
set(FLATBUFFERS_LIB "${TP_LIB_DIR}/libflatbuffers.a")

# 3. 头文件搜索路径
# 这里包含了 arrow, flatbuffers, zstd 和 pod5_format 的所有头文件
include_directories(
    "${TP_INCLUDE_DIR}"
    "${TP_INCLUDE_DIR}/pod5_format/c++"
)

# 4. 基础系统库 (这些通常比较标准，可以直接 find)
find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED) # pod5_format 需要链接 -lz

# 5. 全局编译设置
add_compile_options(-Wall -Wextra -fPIC)

# 6. 设置输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/product/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/product/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/product/lib)

# 7. 构建子目录
add_subdirectory(src)