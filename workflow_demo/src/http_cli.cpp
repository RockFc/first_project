#include "workflow/WFTaskFactory.h"
#include "workflow/WFFacilities.h"
#include <iostream>

// 回调函数，用于处理 HTTP 响应
void http_callback(WFHttpTask *task)
{
    protocol::HttpResponse *resp = task->get_resp();
    const void *body;
    size_t body_len;
    resp->get_parsed_body(&body, &body_len);

    std::cout << "HTTP Response:" << std::endl;
    std::cout << std::string((const char *)body, body_len) << std::endl;
}

int main()
{
    // 创建一个 WaitGroup，等待任务完成
    WFFacilities::WaitGroup wait_group(1);

    // 创建 HTTP GET 请求任务
    WFHttpTask *task = WFTaskFactory::create_http_task("http://localhost:8888/hello", 0, 0, [&wait_group](WFHttpTask *task)
    {
        // 调用 http_callback 处理响应
        http_callback(task);

        // 标记任务完成
        wait_group.done();
    });

    // 启动任务
    task->start();

    // 等待任务完成
    wait_group.wait();

    return 0;
}