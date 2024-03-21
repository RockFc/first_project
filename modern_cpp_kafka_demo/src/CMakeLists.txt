
add_executable(cpp_kafka_test cpp_kafka_test.cpp)
target_link_libraries(cpp_kafka_test PRIVATE ${LIBRDKAFKA_LIBRARY_LIB})