add_executable(http_ws_test http_ws_test.cpp)
add_executable(http_ws_svr http_ws_svr.cpp)
add_executable(http_ws_cli http_ws_cli.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_SYSTEM_NAME MATCHES "Windows") # mingw-w64
    target_link_libraries(http_ws_test wsock32 ws2_32)
    target_link_libraries(http_ws_svr wsock32 ws2_32)
    target_link_libraries(http_ws_cli wsock32 ws2_32)
endif()