
# 最低版本需求
cmake_minimum_required(VERSION 3.12)

# 工程名
set (PROJECT_NAME libhv_demo)
PROJECT(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 检查是否定义了构建类型，如果没有定义，设置默认值为 Release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif()

# 设置option开关默认值
option(BUILD_SRC "Builds src" ON)
option(BUILD_TEST "Builds testase" ON)
option(BUILD_3T "Builds third party testcase" OFF)
option(BUILD_UT "Builds unit testcase" OFF)
option(BUILD_IT "Builds interface testcase" ON)
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

find_package(Threads REQUIRED)
link_libraries(Threads::Threads)

# 指定 libhv 头文件路径
set (LIBHV_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/../../libhv/build/include)
# 指定 libhv 库的路径
set(LIBHV_LIB ${PROJECT_SOURCE_DIR}/../../libhv/build/lib/libhv.so)

# 指定 libmodbus 头文件路径
set (LIBMODBUS_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/../../libmodbus/build/include)
# 指定 libmodbus 库的路径
set(LIBMODBUS_LIB ${PROJECT_SOURCE_DIR}/../../libmodbus/build/lib/libmodbus.so)

# 添加头文件路径
include_directories(
    ${LIBHV_INCLUDE_DIR}
    ${LIBMODBUS_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/src
    )

#设置编译选项
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -Wall -fPIC -pipe -std=gnu90")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC -pipe -lpthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fexceptions")
# gcov 编译选项
if (ENABLE_GCOV)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage -lgcov")

    message(STATUS "Build with gcov")
endif()


#设置输出文件路径
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/product/bin)
set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/product/lib)


#打印环境信息
message("工程名: " ${PROJECT_NAME})
message("工程目录: " ${PROJECT_SOURCE_DIR})
message("编译后可执行程序目录: " ${EXECUTABLE_OUTPUT_PATH})
message("编译后库程序文件目录: " ${LIBRARY_OUTPUT_PATH})

if(BUILD_SRC)
    add_subdirectory(src)
endif()
if(BUILD_TEST)
    add_subdirectory(test)
endif()