add_executable(rpc_client client.cpp)
add_executable(rpc_server server.cpp rpc_service.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_SYSTEM_NAME MATCHES "Windows") # mingw-w64
  target_link_libraries(rpc_client wsock32 ws2_32)
  target_link_libraries(rpc_server wsock32 ws2_32)
endif()

