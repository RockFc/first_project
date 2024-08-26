#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <thread>

void foo()
{
    SPDLOG_INFO("in foo()");
}

class SpdlogTest
{
public:
    SpdlogTest(/* args */) = default;
    ~SpdlogTest() = default;
    void show()
    {
        SPDLOG_INFO("in SpdlogTest::show()");
    }
};

// 获取当前时间，并生成带时间标识的文件名
std::string generate_log_filename() 
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_c), "%Y_%m_%d_%H_%M_%S");
    return "async_log_" + ss.str() + ".log";
}

int main() {
    // 创建一个异步的日志器，写入到 "async_log.txt" 文件中
    auto logger = spdlog::basic_logger_mt<spdlog::async_factory>("async_logger", generate_log_filename(), true);
    
    // 设置为全局的默认日志器
    spdlog::set_default_logger(logger);
    
    // 设置日志级别
    spdlog::set_level(spdlog::level::debug);
    
    // 设置日志格式
    // spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [thread %t] %v");
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e][%L][%P][%t][%s][%!] %v");
    
    // 使用 SPDLOG_INFO 宏记录日志
    SPDLOG_INFO("This is an info message.");
    SPDLOG_WARN("This is a warning message.");
    SPDLOG_ERROR("This is an error message with param {}", 42);

    SPDLOG_INFO("Some info message with param {}", 42);
    SPDLOG_DEBUG("Some debug message");

    foo();

    SpdlogTest test;
    test.show();

    // 模拟一些异步操作
    std::thread t1([]() {
        SPDLOG_INFO("I am in thread t1.");
    });

    t1.join();

    // 强制刷新缓冲区中的日志
    spdlog::default_logger()->flush();

    return 0;
}