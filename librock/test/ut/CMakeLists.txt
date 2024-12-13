
set (SRC_DIR ${PROJECT_SOURCE_DIR}/src)

include_directories(${SRC_DIR})
aux_source_directory(${PROJECT_SOURCE_DIR}/src src)

add_executable( ut_util util_test.cpp ${src})
target_link_libraries(ut_util )
