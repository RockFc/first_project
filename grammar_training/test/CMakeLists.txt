# 打印当前目录
message("测试工程当前目录: "  ${CMAKE_CURRENT_SOURCE_DIR})

# 需包含的头文件
include_directories(
    ${PROJECT_SOURCE_DIR}/src
)

find_package(Threads)