
# 最低版本需求
cmake_minimum_required(VERSION 2.8)

# 工程名
set (PROJECT_NAME algo_training)
PROJECT(${PROJECT_NAME})

# 设置option开关默认值
option(BUILD_All "Builds all projects" ON)
option(BUILD_TEST "Builds test projects" ON)
option(ENABLE_GCOV "Enable gcov (debug, Linux build only)." OFF)

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

#设置编译选项
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -Wall -fPIC -pipe -std=gnu90")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC -pipe -std=gnu++14 -lpthread -fno-exceptions -Wno-invalid-offsetof")
if (ENABLE_GCOV)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage -lgcov")
endif()

#设置输出文件路径
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/product/bin)
set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/product/lib)

#打印环境信息
message("工程名: " ${PROJECT_NAME})
message("工程目录: " ${PROJECT_SOURCE_DIR})
message("KDS_MOBILE_STOCK_HOME目录: " $ENV{KDS_MOBILE_STOCK_HOME})
message("编译后可执行程序目录: " ${EXECUTABLE_OUTPUT_PATH})
message("编译后库程序文件目录: " ${LIBRARY_OUTPUT_PATH})

add_executable(linked_list linked_list.cpp)
add_executable(sort_algo sort_algo.cpp)