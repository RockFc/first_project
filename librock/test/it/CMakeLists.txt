
set (SRC_DIR ${PROJECT_SOURCE_DIR}/src)

include_directories(${SRC_DIR})

add_executable( it_test it_test.cpp)
target_link_libraries(it_test rock)
