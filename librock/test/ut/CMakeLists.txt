
set (SRC_DIR ${PROJECT_SOURCE_DIR}/src)

include_directories(${SRC_DIR})
aux_source_directory(${PROJECT_SOURCE_DIR}/src src)

add_executable( ut_threadpool_1 ut_threadpool_1.cpp ${src})
target_link_libraries(ut_threadpool_1 gtest gtest_main)

add_executable( ut_filepool ut_filepool.cpp ${src})
target_link_libraries(ut_filepool gtest gtest_main)

add_executable( ut_future_1 ut_future_1.cpp ${src})
target_link_libraries(ut_future_1 gtest gtest_main)