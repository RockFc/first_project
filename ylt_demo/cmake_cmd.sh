mkdir -p build && cd build
rm -rf *
cmake -D BUILD_All=ON  .. && cmake --build . -j10
