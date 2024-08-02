
message("WORKFLOW_LIB: " ${WORKFLOW_LIB})

add_executable(http_svr http_svr.cpp )
target_link_libraries(http_svr ${WORKFLOW_LIB})