#include "workflow/WFHttpServer.h"
#include "workflow/WFFacilities.h"
#include <ctime>

// 处理 "/hello" 请求
void hello_process(WFHttpTask *task)
{
    protocol::HttpResponse *resp = task->get_resp();
    resp->set_status_code("200");
    resp->append_output_body("<html>Hello, World!</html>");
}

// 处理 "/time" 请求
void time_process(WFHttpTask *task)
{
    protocol::HttpResponse *resp = task->get_resp();
    resp->set_status_code("200");

    std::time_t now = std::time(nullptr);
    char buf[100];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    
    std::string body = "<html>Current server time: ";
    body += buf;
    body += "</html>";

    resp->append_output_body(body);
}

int main()
{
    WFHttpServer server([](WFHttpTask *task) {
        const char *uri = task->get_req()->get_request_uri();
        if (strcmp(uri, "/hello") == 0)
        {
            hello_process(task);
        }
        else if (strcmp(uri, "/time") == 0)
        {
            time_process(task);
        }
        else
        {
            protocol::HttpResponse *resp = task->get_resp();
            resp->set_status_code("404");
            resp->append_output_body("<html>404 Not Found</html>");
        }
    });

    if (server.start(8888) == 0)
    {
        WFFacilities::WaitGroup wait_group(1);
        wait_group.wait();
        server.stop();
    }
    else
    {
        fprintf(stderr, "Cannot start server\n");
        exit(1);
    }

    return 0;
}

/*
	•	http://localhost:8888/hello：应显示 “Hello, World!”。
	•	http://localhost:8888/time：应显示当前服务器时间。
*/