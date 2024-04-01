# 打印当前目录
message("测试工程当前目录: "  ${CMAKE_CURRENT_SOURCE_DIR})

# 需包含的头文件
include_directories(
    ${PROJECT_SOURCE_DIR}/src
)

find_package(Threads)

add_executable(container_test_1 container_test_1.cpp ${src})
target_link_libraries(container_test_1 gtest gtest_main ${CMAKE_THREAD_LIBS_INIT}) 

add_executable(set_vec_mem set_vector_mem.cpp ${src})
target_link_libraries(set_vec_mem gtest gtest_main ${CMAKE_THREAD_LIBS_INIT}) 

add_executable(to_string_test to_string_test.cpp ${src})
target_link_libraries(to_string_test ${CMAKE_THREAD_LIBS_INIT}) 