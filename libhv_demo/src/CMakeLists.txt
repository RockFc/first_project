

add_executable(upd_cli udp_client_test.cpp )
target_link_libraries(upd_cli  ${LIBHV_LIB})

add_executable(upd_svr udp_server_test.cpp )
target_link_libraries(upd_svr  ${LIBHV_LIB})