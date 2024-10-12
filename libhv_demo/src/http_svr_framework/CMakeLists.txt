
add_executable(http_svr_framework main.cpp wy_http_svr.cpp wy_http_message.cpp)
target_link_libraries(http_svr_framework  ${LIBHV_LIB})