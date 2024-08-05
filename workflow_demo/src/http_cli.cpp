#include "workflow/WFFacilities.h"
#include "workflow/WFTaskFactory.h"
#include <chrono>
#include <iostream>
#include <thread>



// 回调函数，用于处理 HTTP 响应
void http_callback(WFHttpTask* task)
{
    protocol::HttpResponse* resp = task->get_resp();
    const void*             body;
    size_t                  body_len;
    resp->get_parsed_body(&body, &body_len);

    protocol::HttpRequest* req = task->get_req();

    std::cout << req->get_request_uri() << " HTTP Response:" << std::endl;
    std::cout << std::string(( const char* )body, body_len) << std::endl;
}

int main()
{
    // 创建一个 WaitGroup，等待任务完成
    WFFacilities::WaitGroup wait_group(2);

    // 创建 HTTP GET 请求任务
    auto task_hello = WFTaskFactory::create_http_task("http://localhost:8888/hello", 0, 0,
                                                       [ &wait_group ](WFHttpTask* task)
                                                       {
                                                           http_callback(task);
                                                           wait_group.done();
                                                       });

    // 启动任务
    task_hello->start();

    // 等待 1 秒
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 创建 HTTP GET 请求任务
    auto task_time = WFTaskFactory::create_http_task("http://localhost:8888/time", 0, 0,
                                                         [ &wait_group ](WFHttpTask* task)
                                                         {
                                                             http_callback(task);
                                                             wait_group.done();
                                                         });
    // 启动任务
    task_time->start();

    // 等待任务完成
    wait_group.wait();

    return 0;
}