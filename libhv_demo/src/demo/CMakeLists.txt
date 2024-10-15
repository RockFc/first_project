

add_executable(udp_cli udp_cli.cpp )
target_link_libraries(udp_cli  ${LIBHV_LIB})

add_executable(udp_svr udp_svr.cpp )
target_link_libraries(udp_svr  ${LIBHV_LIB})

add_executable(modbus_tcp_cli modbus_tcp_cli.cpp )
target_link_libraries(modbus_tcp_cli  ${LIBMODBUS_LIB})

add_executable(modbus_tcp_svr modbus_tcp_svr.cpp )
target_link_libraries(modbus_tcp_svr  ${LIBMODBUS_LIB})

add_executable(ev_loop ev_loop.cpp )
target_link_libraries(ev_loop  ${LIBHV_LIB})

add_executable(ev_loop_thread ev_loop_thread.cpp )
target_link_libraries(ev_loop_thread  ${LIBHV_LIB})

add_executable(ev_loop_thread_pool ev_loop_thread_pool.cpp )
target_link_libraries(ev_loop_thread_pool  ${LIBHV_LIB})

add_executable(defer_test defer_test.cpp )
target_link_libraries(defer_test  ${LIBHV_LIB})

add_executable(hpath_test hpath_test.cpp )
target_link_libraries(hpath_test  ${LIBHV_LIB})

add_executable(hstring_test hstring_test.cpp )
target_link_libraries(hstring_test  ${LIBHV_LIB})

add_executable(hthread_test hthread_test.cpp )
target_link_libraries(hthread_test  ${LIBHV_LIB})

add_executable(objectpool_test objectpool_test.cpp )
target_link_libraries(objectpool_test  ${LIBHV_LIB})

add_executable(sizeof_test sizeof_test.cpp )
target_link_libraries(sizeof_test  ${LIBHV_LIB})

add_executable(threadpool_test threadpool_test.cpp )
target_link_libraries(threadpool_test  ${LIBHV_LIB})

add_executable(json_test json_test.cpp )
target_link_libraries(json_test  ${LIBHV_LIB})

add_executable(ws_svr ws_svr.cpp )
target_link_libraries(ws_svr  ${LIBHV_LIB})

add_executable(ws_svr_bin ws_svr_bin.cpp)
target_link_libraries(ws_svr_bin  ${LIBHV_LIB})

add_executable(ws_cli_bin ws_cli_bin.cpp)
target_link_libraries(ws_cli_bin  ${LIBHV_LIB})
