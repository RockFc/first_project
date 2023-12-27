mkdir -p build && cd build
rm -rf *
cmake -D BUILD_All=ON -D CMAKE_BUILD_TYPE=Debug -D ENABLE_GCOV=1 .. && make