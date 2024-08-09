add_executable(coroutine_test coroutine_test.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_SYSTEM_NAME MATCHES "Windows") # mingw-w64
    target_link_libraries(coroutine_test wsock32 ws2_32)
endif()