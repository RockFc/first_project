# 打印当前目录
message("测试工程当前目录: "  ${CMAKE_CURRENT_SOURCE_DIR})

# 需包含的头文件
INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/src
)

# 需包含的库文件
#link_directories(
#    ${PROJECT_SOURCE_DIR}/project/lib
#)

find_package(Threads)

#设置源文件目录
#这里将src中生成的lib连接进来了，所以不需要包含src中的源文件
AUX_SOURCE_DIRECTORY(. test)
aux_source_directory(${PROJECT_SOURCE_DIR}/src src)
#过滤源文件中的某些文件
list(REMOVE_ITEM src "${PROJECT_SOURCE_DIR}/src/main.cpp")


add_executable(s_ut_common ut_common.cpp ${src})
TARGET_LINK_LIBRARIES(s_ut_common gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})

add_executable(s_ut_counter_1 ut_counter_1.cpp ${src})
TARGET_LINK_LIBRARIES(s_ut_counter_1 gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})

add_executable(s_ut_counter_2 ut_counter_2.cpp ${src})
TARGET_LINK_LIBRARIES(s_ut_counter_2 gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})

#TARGET_LINK_LIBRARIES(s_ut_common ${CMAKE_THREAD_LIBS_INIT})