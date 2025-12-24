add_executable(pod5_reader pod5_reader.cpp)

target_link_libraries(pod5_reader PRIVATE 
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
set_target_properties(pod5_reader PROPERTIES 
    INSTALL_RPATH "$ORIGIN/../../thirdparty/pod5/lib"
    BUILD_WITH_INSTALL_RPATH TRUE
)


add_executable(pod5_writer pod5_writer.cpp)

target_link_libraries(pod5_writer PRIVATE 
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

# 确保在 BUILD 和 INSTALL 时都包含正确的 RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# 使用绝对路径（临时测试）或更健壮的 ORIGIN 
# 注意：在某些 shell 下，$ORIGIN 需要转义为 \${ORIGIN}
set_target_properties(pod5_writer PROPERTIES 
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "\$ORIGIN/../../thirdparty/pod5/lib"
)


add_executable(pod5_writer_easy pod5_writer_easy.cpp)

target_link_libraries(pod5_writer_easy PRIVATE 
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

# 确保在 BUILD 和 INSTALL 时都包含正确的 RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# 使用绝对路径（临时测试）或更健壮的 ORIGIN 
# 注意：在某些 shell 下，$ORIGIN 需要转义为 \${ORIGIN}
set_target_properties(pod5_writer_easy PROPERTIES 
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "\$ORIGIN/../../thirdparty/pod5/lib"
)