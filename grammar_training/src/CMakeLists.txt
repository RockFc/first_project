add_executable(sp_test shared_ptr_about.cpp)
target_link_libraries(sp_test pthread)

add_executable(vt_test variable_temple.cpp)

add_executable(t_test_1 template_test_1.cpp)
add_executable(t_test_2 template_test_2.cpp)

add_executable(l_test_1 lamdba_test_1.cpp)

add_executable(pq_test_1 priority_queue_1.cpp)

add_executable(instruct_1 instruction.cpp)

add_executable(xiaopen_test_1 xiaopen_test_1.cpp)
target_link_libraries(xiaopen_test_1 pthread)