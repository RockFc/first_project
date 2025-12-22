# 假设你的可执行程序叫 pod5_demo
add_executable(pod5_demo main.cpp)

target_include_directories(pod5_demo PRIVATE ${PROJECT_SOURCE_DIR}/src)

# 链接顺序：业务 -> pod5 -> 基础库
target_link_libraries(pod5_demo PRIVATE 
    pod5_format 
    Arrow::arrow_shared      # 使用现代 target 形式
    /usr/lib/x86_64-linux-gnu/libcurl.so
    flatbuffers::flatbuffers 
    zstd::libzstd_shared
    ZLIB::ZLIB
    Threads::Threads
)