

add_executable(rpc_sevice rpc_service_test.cpp )
target_link_libraries(rpc_sevice ${ANANAS_RPC_LIB} ${ANANAS_NET_LIB} ${ANANAS_UTIL_LIB} ${LLHTTP_LIB})

add_executable(rpc_client rpc_client_test.cpp )
target_link_libraries(rpc_client ${ANANAS_RPC_LIB} ${ANANAS_NET_LIB} ${ANANAS_UTIL_LIB} ${LLHTTP_LIB})
