# 打印当前目录
message("测试工程当前目录: "  ${CMAKE_CURRENT_SOURCE_DIR})

# 需包含的头文件
include_directories(
    ${PROJECT_SOURCE_DIR}/src
)

find_package(Threads)

add_executable(container_test_1 container_test_1.cpp ${src})
target_link_libraries(container_test_1 gtest gtest_main ${CMAKE_THREAD_LIBS_INIT}) 