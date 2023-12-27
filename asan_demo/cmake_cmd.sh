mkdir -p build && cd build
rm -rf *

# 编译asan_test
# cmake -D CMAKE_BUILD_TYPE=Debug -D ENABLE_ASAN=ON  -D ASAN_TEST=ON  .. && make

# 编译valgrind_test
# 使用 valgrind 也可方便定位内存消耗等相关问题（关闭ASAN开关）
cmake  -D CMAKE_BUILD_TYPE=Debug -D VALGRIND_TEST=ON .. && make
# valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all asan_test
