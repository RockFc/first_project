mkdir -p build && cd build
rm -rf *
# cmake -D CMAKE_BUILD_TYPE=Debug  .. && make
cmake -D CMAKE_BUILD_TYPE=Release  -D BUILD_SRC=OFF .. && make

