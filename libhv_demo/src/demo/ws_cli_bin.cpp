#include "ws_svr_bin.h"
#include <hv/HttpClient.h>
#include <iostream>

int main()
{
    // 创建 HttpClient 对象
    hv::HttpClient client;

    // 设置 URL
    const char* url = "http://localhost:10080/post_bin";

    // 准备要发送的二进制数据
    MyStruct original = {42, "Hello, World!", 3.14159};

    // 序列化结构体
    size_t dataSize;
    void*  serializedData = serialize(&original, &dataSize);
    if (!serializedData)
    {
        printf("Failed to serialize data.\n");
        return -1;
    }

    // 将二进制数据存储到 std::string 中
    std::string body(reinterpret_cast<const char*>(serializedData), dataSize);

    // 创建 HTTP 请求
    HttpRequestPtr req(new HttpRequest);
    req->method = HTTP_POST;
    req->url    = url;
    req->body   = body;
    req->content_type
        = http_content_type::APPLICATION_OCTET_STREAM;  // 设置 Content-Type 为二进制数据

    // 发送请求并接收响应
    HttpResponsePtr resp(new HttpResponse);
    auto            ret = client.send(req.get(), resp.get());
    if (0 != ret)
    {
        std::cerr << "Request failed" << std::endl;
        return -1;
    }

    // 输出响应状态码和正文
    std::cout << "Response status: " << resp->status_code << std::endl;
    MyStruct deserialized = deserialize(resp->body.data(), resp->body.size());
    printMyStruct(&deserialized);

    return 0;
}