
set (SRC_DIR ${PROJECT_SOURCE_DIR}/src)

include_directories(${SRC_DIR})
aux_source_directory(${PROJECT_SOURCE_DIR}/src src)

add_executable( ut_test ut_test.cpp ${src})

add_executable( ut_filepool ut_filepool.cpp ${src})

add_executable( ut_filepool_2 ut_filepool_2.cpp ${src})
