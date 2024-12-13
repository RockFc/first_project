mkdir -p build && cd build
rm -rf *
cmake -D BUILD_All=ON -D BUILD_TEST=ON -D BUILD_UT=ON -D BUILD_IT=ON-D BUILD_3T=OFF  -D CMAKE_BUILD_TYPE=Debug -D ENABLE_GCOV=ON .. && make
