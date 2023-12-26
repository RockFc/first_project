mkdir -p build && cd build
cmake -D BUILD_All=ON -D CMAKE_BUILD_TYPE=Debug -D ENABLE_GCOV=1 .. && make