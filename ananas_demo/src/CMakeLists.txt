

add_executable(net_sevice net_service_test.cpp )
target_link_libraries(net_sevice ${ANANAS_NET_LIB} ${ANANAS_UTIL_LIB} ${LLHTTP_LIB})

add_executable(net_client net_client_test.cpp )
target_link_libraries(net_client ${ANANAS_NET_LIB} ${ANANAS_UTIL_LIB} ${LLHTTP_LIB})

add_executable( util_test util_test.cpp )
target_link_libraries(util_test ${ANANAS_UTIL_LIB})

add_executable( ev_test event_loop_test.cpp )
target_link_libraries(ev_test ${ANANAS_NET_LIB} ${ANANAS_UTIL_LIB} ${LLHTTP_LIB})

add_executable( rpc_service rpc_service_test.cpp)
target_link_libraries(rpc_service ${PB_LIB} ${ANANAS_RPC_LIB} ${ANANAS_NET_LIB} ${ANANAS_UTIL_LIB} ${LLHTTP_LIB} ${PROTOBUF_LIB})

add_executable( rpc_client rpc_client_test.cpp)
target_link_libraries(rpc_client ${PB_LIB} ${ANANAS_RPC_LIB} ${ANANAS_NET_LIB} ${ANANAS_UTIL_LIB} ${LLHTTP_LIB} ${PROTOBUF_LIB})