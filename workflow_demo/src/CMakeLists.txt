
message("WORKFLOW_LIB: " ${WORKFLOW_LIB})

add_executable(http_svr http_svr.cpp )
target_link_libraries(http_svr ${WORKFLOW_LIB})

add_executable(http_cli http_cli.cpp )
target_link_libraries(http_cli ${WORKFLOW_LIB})

add_executable(http_svr_json http_svr_json.cpp )
target_link_libraries(http_svr_json ${WORKFLOW_LIB})

add_executable(http_cli_json http_cli_json.cpp )
target_link_libraries(http_cli_json ${WORKFLOW_LIB})