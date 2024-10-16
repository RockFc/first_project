# 指定 auto_ep 头文件路径
set (HTTP_SVR_FW_DIR ${PROJECT_SOURCE_DIR}/src/http_svr_framework)

# 需包含的头文件
INCLUDE_DIRECTORIES(
    ${HTTP_SVR_FW_DIR}
)

aux_source_directory(${HTTP_SVR_FW_DIR} http_svr_fw_src)
#过滤源文件中的某些文件
list(REMOVE_ITEM http_svr_fw_src "${HTTP_SVR_FW_DIR}/main.cpp")

add_executable(it_http_svr ItHttpServer.cpp ${http_svr_fw_src})
target_link_libraries(it_http_svr gtest gtest_main ${LIBHV_LIB})

add_executable(it_ws_svr ItWsServer.cpp ${http_svr_fw_src})
target_link_libraries(it_ws_svr gtest gtest_main ${LIBHV_LIB} ${LIBUV_LIB})