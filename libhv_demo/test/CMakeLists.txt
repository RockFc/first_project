# 当前系统已安装gtest，则使用系统自带的gtest
# # 指定 googletest 头文件路径
# set (GTEST_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/third_party/googletest/include)
# # 指定 googletest 库的路径
# set(GTEST_LIB_PATH ${PROJECT_SOURCE_DIR}/third_party/googletest/lib)

# # 添加头文件路径
# include_directories(
#     ${GTEST_INCLUDE_DIR}
#     )

# # 需包含的库文件
# link_directories(
#    ${GTEST_LIB_PATH}
#    )

#设置输出文件路径
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/product/test)

if(BUILD_3T)
    add_subdirectory(3t)
endif()
if(BUILD_UT)
    add_subdirectory(ut)
endif()
if(BUILD_IT)
    add_subdirectory(it)
endif()