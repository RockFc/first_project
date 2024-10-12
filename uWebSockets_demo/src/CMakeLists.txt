message("LIBUSOCKETS_LIB: " ${LIBUSOCKETS_LIB})

add_executable(hello hello.cpp)
target_link_libraries(hello ${LIBUSOCKETS_LIB} OpenSSL::SSL ZLIB::ZLIB)

add_executable(ws_svr ws_svr.cpp)
target_link_libraries(ws_svr ${LIBUSOCKETS_LIB} OpenSSL::SSL ZLIB::ZLIB)