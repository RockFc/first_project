
# 最低版本需求
cmake_minimum_required(VERSION 3.12)

# 工程名
set (PROJECT_NAME rpc_demo)
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


#设置输出文件路径
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/product/bin)
set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/product/lib)

#设置C++标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#打印环境信息
message("工程名: " ${PROJECT_NAME})
message("工程目录: " ${PROJECT_SOURCE_DIR})
message("编译后可执行程序目录: " ${EXECUTABLE_OUTPUT_PATH})
message("编译后库程序文件目录: " ${LIBRARY_OUTPUT_PATH})


# 查找 Protobuf 库
find_package(Protobuf REQUIRED)

# 设置 .proto 文件所在文件夹
set(PROTO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src_proto)

# 查找 .proto 文件
file(GLOB PROTO_FILES "${PROTO_DIR}/*.proto")

# 设置生成的 Protocol Buffers C++ 文件输出路径
set(PROTO_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/product/proto)

# 创建输出文件夹
file(MAKE_DIRECTORY ${PROTO_OUTPUT_DIR})

# 生成 Protobuf 文件
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES} PROTOC_OUT_DIR ${PROTO_OUTPUT_DIR})

# 创建静态库
add_library(proto_lib STATIC ${PROTO_SRCS} ${PROTO_HDRS})