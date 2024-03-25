
# 最低版本需求
cmake_minimum_required(VERSION 3.12)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

set (PROJECT_NAME cppkafka_demo)
PROJECT(${PROJECT_NAME})

# 查找依赖
find_package(Boost REQUIRED COMPONENTS program_options)

find_package(CppKafka REQUIRED)

#设置输出文件路径
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/product/bin)
set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/product/lib)

#打印环境信息
message("工程名: " ${PROJECT_NAME})
message("工程目录: " ${PROJECT_SOURCE_DIR})
message("编译后可执行程序目录: " ${EXECUTABLE_OUTPUT_PATH})
message("编译后库程序文件目录: " ${LIBRARY_OUTPUT_PATH})

add_subdirectory(src)