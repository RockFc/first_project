#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
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

void http_json_callback(WFHttpTask* task)
{
    protocol::HttpRequest*  req  = task->get_req();
    protocol::HttpResponse* resp = task->get_resp();

    if (resp->get_status_code() == std::string("200"))
    {
        const void* body;
        size_t      body_len;
        resp->get_parsed_body(&body, &body_len);

        rapidjson::Document doc;
        doc.Parse(( const char* )body, body_len);
        if (doc.HasParseError())
        {
            std::cerr << req->get_request_uri() << " response body parse failed." << std::endl;
            return;
        }
        
        // 访问并打印解析后的数据
        if (doc.HasMember("err_msg") && doc[ "err_msg" ].IsString())
        {
            std::cout << "err_msg: " << doc[ "err_msg" ].GetString() << std::endl;
        }
        if (doc.HasMember("err_code") && doc[ "err_code" ].IsInt())
        {
            std::cout << "err_code: " << doc[ "err_code" ].GetInt() << std::endl;
        }

        if (doc.HasMember("data") && doc[ "data" ].IsObject())
        {
            const rapidjson::Value& data = doc[ "data" ];
            if (data.HasMember("city") && data[ "city" ].IsString())
            {
                std::cout << "City: " << data[ "city" ].GetString() << std::endl;
            }
            if (data.HasMember("zipcode") && data[ "zipcode" ].IsString())
            {
                std::cout << "Zipcode: " << data[ "zipcode" ].GetString() << std::endl;
            }
        }
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

    std::this_thread::sleep_for(std::chrono::seconds(1));
    WFHttpTask* task_info = WFTaskFactory::create_http_task(url_prefix + "/info", 0, 0, http_json_callback);
    task_info->start();

    getchar();  // 保持程序运行等待回调完成

    return 0;
}