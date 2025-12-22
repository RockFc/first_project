add_executable(pod5_demo main.cpp)

target_link_libraries(pod5_demo PRIVATE 
    ${POD5_LIB}
    ${ARROW_LIB}
    ${CURL_LIB}
    ${FLATBUFFERS_LIB}
    ZLIB::ZLIB
    Threads::Threads
    zstd
    dl              # Arrow 必须
    rt              # 实时库
    ssl             # Curl/Arrow 可能需要
    crypto          # Curl/Arrow 可能需要
)

# 解决运行时找不到 .so 的问题：设置 RPATH
# 让可执行程序运行时优先去 ../../thirdparty/pod5/lib 找 libarrow.so 和 libcurl.so
set_target_properties(pod5_demo PROPERTIES 
    INSTALL_RPATH "$ORIGIN/../../thirdparty/pod5/lib"
    BUILD_WITH_INSTALL_RPATH TRUE
)