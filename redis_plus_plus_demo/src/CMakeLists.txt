

add_executable(rpp_test redis_p_p_test.cpp )
target_link_libraries(rpp_test ${REDIS_PLUS_PLUS_LIB} ${HIREDIS_LIB})