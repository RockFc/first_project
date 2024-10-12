
# 最低版本需求
cmake_minimum_required(VERSION 3.15)

# 工程名
set (PROJECT_NAME http_ws_test)
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

# 查找库文件
find_package(Threads REQUIRED)
link_libraries(Threads::Threads)

# 添加头文件路径
include_directories(
    ${PROJECT_SOURCE_DIR}
    ../../yalantinglibs/include
    ../../yalantinglibs/include/ylt/thirdparty
    ../../yalantinglibs/include/ylt/standalone
    )

#设置编译选项
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines")
    #-ftree-slp-vectorize with coroutine cause link error. disable it util gcc fix.
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-tree-slp-vectorize")
endif()


#设置输出文件路径
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/product/bin)
set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/product/lib)

#打印环境信息
message("工程名: " ${PROJECT_NAME})
message("工程目录: " ${PROJECT_SOURCE_DIR})
message("编译后可执行程序目录: " ${EXECUTABLE_OUTPUT_PATH})
message("编译后库程序文件目录: " ${LIBRARY_OUTPUT_PATH})

add_subdirectory(src)