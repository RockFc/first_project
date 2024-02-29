

add_executable(net_sevice net_service_test.cpp )
target_link_libraries(net_sevice ${ANANAS_RPC_LIB} ${ANANAS_NET_LIB} ${ANANAS_UTIL_LIB} ${LLHTTP_LIB})

add_executable(net_client net_client_test.cpp )
target_link_libraries(net_client ${ANANAS_RPC_LIB} ${ANANAS_NET_LIB} ${ANANAS_UTIL_LIB} ${LLHTTP_LIB})

add_executable( util_test util_test.cpp )
target_link_libraries(util_test ${ANANAS_UTIL_LIB})

