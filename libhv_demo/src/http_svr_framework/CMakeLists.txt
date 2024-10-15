
aux_source_directory(. src)

add_executable(http_svr_framework ${src})
target_link_libraries(http_svr_framework  ${LIBHV_LIB})