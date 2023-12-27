if(ASAN_TEST)
    add_executable(asan_test asan_test.cpp)
    message("ASAN_TEST opened.")
endif()

if(VALGRIND_TEST)
    add_executable(valgrind_test valgrind_test.cpp)
    message("VALGRIND_TEST opened.")
endif()

