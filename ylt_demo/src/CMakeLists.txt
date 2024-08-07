add_executable(http_ws_test http_ws_test.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_SYSTEM_NAME MATCHES "Windows") # mingw-w64
    target_link_libraries(http_ws_test wsock32 ws2_32)
endif()