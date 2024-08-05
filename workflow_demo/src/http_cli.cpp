#include "workflow/WFFacilities.h"
#include "workflow/WFTaskFactory.h"
#include <chrono>
#include <iostream>
#include <thread>

void http_callback(WFHttpTask* task)
{
    protocol::HttpRequest*  req  = task->get_req();
    protocol::HttpResponse* resp = task->get_resp();

    if (resp->get_status_code() == std::string("200"))
    {
        const void* body;
        size_t      body_len;
        resp->get_parsed_body(&body, &body_len);
        std::cout << req->get_request_uri() << " response body: " << std::string(( const char* )body, body_len) << std::endl;
    }
    else
    {
        std::cerr << req->get_request_uri() << " request failed with status: " << resp->get_status_code() << std::endl;
    }
}

int main()
{
    const std::string url_prefix = "http://localhost:8888";
    WFHttpTask*       task_hello = WFTaskFactory::create_http_task(url_prefix + "/hello", 0, 0, http_callback);
    task_hello->start();

    std::this_thread::sleep_for(std::chrono::seconds(1));
    WFHttpTask* task_time = WFTaskFactory::create_http_task(url_prefix + "/time", 0, 0, http_callback);
    task_time->start();

    std::this_thread::sleep_for(std::chrono::seconds(1));
    WFHttpTask* task_err = WFTaskFactory::create_http_task(url_prefix + "/unknown", 0, 0, http_callback);
    task_err->start();

    getchar();  // 保持程序运行等待回调完成

    return 0;
}