cmake_minimum_required(VERSION 3.12)
project(pod5_format_demo)

# 1. 基础编译选项
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 默认 Release 模式
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# 2. 查找基础系统依赖 (现代 CMake 会自动处理 include 和 libs)
find_package(Threads REQUIRED)
find_package(CURL REQUIRED)
find_package(Arrow REQUIRED)
find_package(Flatbuffers REQUIRED)
find_package(ZLIB REQUIRED)
find_package(zstd REQUIRED CONFIG)

# 3. 配置 POD5 路径 (假设你没有执行 sudo make install)
set(POD5_ROOT_DIR "${PROJECT_SOURCE_DIR}/../../pod5-file-format_1")

# 定义导入库
add_library(pod5_format STATIC IMPORTED)
set_target_properties(pod5_format PROPERTIES
    IMPORTED_LOCATION "${POD5_ROOT_DIR}/build/c++/libpod5_format.a"
    INTERFACE_INCLUDE_DIRECTORIES "${POD5_ROOT_DIR}/c++;${POD5_ROOT_DIR}/build/c++"
)

# 4. 全局编译设置 (移除重复和冲突的 flag)
add_compile_options(-Wall -Wextra -fPIC)

if (ENABLE_GCOV AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(--coverage)
    add_link_options(--coverage)
    message(STATUS "Build with gcov enabled")
endif()

# 5. 设置输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/product/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/product/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/product/lib)

# 6. 构建子目录
if(BUILD_SRC)
    add_subdirectory(src)
endif()