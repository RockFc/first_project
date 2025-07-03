include_directories(
    ${PROJECT_SOURCE_DIR}/../../3rd_party_manager/out/googletest/include
    )
# 设置库依赖
set (GTEST_LIBS ${PROJECT_SOURCE_DIR}/../../3rd_party_manager/out/googletest/lib/libgtest.a  
                ${PROJECT_SOURCE_DIR}/../../3rd_party_manager/out/googletest/lib/libgtest_main.a)

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