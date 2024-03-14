
message("boost 头文件: " ${Boost_INCLUDE_DIRS})
message("boost 库文件: " ${Boost_LIBRARIES})

add_executable(asio_time_test asio_time_test.cpp)
target_include_directories(asio_time_test PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(asio_time_test PRIVATE ${Boost_LIBRARIES})