#include "hv/json.hpp"
#include <fstream>
#include <iostream>

void create_read_write_json()
{
    // 创建 JSON 对象
    nlohmann::json json_obj;
    json_obj["code"] = 0;
    json_obj["msg"]  = "";

    nlohmann::json data_obj;
    data_obj["server_name"] = "ep";
    data_obj["version"]     = "0.0.1";

    json_obj["data"] = data_obj;

    // 输出 JSON 字符串
    std::cout << json_obj.dump(4) << std::endl;  // 4 表示缩进层级

    // 读取 JSON 字符串
    std::string svr_name = json_obj["data"]["server_name"];
    std::cout << "server_name: " << svr_name << std::endl;
    int code = json_obj["code"];
    std::cout << "code: " << code << std::endl;

    // 修改 JSON 字符串
    json_obj["code"]            = 1;
    json_obj["msg"]             = "success";
    json_obj["data"]["version"] = "0.0.2";
    std::cout << json_obj.dump(4) << std::endl;
}

void read_json_file()
{
    // 定义 JSON 文件路径
    std::string filename = "config.json";
    // 创建一个输入文件流
    std::ifstream inputFile(filename);
    // 检查文件是否打开成功
    if (!inputFile.is_open())
    {
        std::cerr << "Could not open the file: " << filename << std::endl;
        return;
    }
    // 解析 JSON 文件
    nlohmann::json json_obj;
    inputFile >> json_obj;

    // 关闭文件流
    inputFile.close();

    // 输出整个 JSON 对象
    std::cout << "Parsed JSON:\n" << json_obj.dump(4) << std::endl;

    if (json_obj.contains("code"))
    {
        int code = json_obj["code"];
        std::cout << "code: " << code << std::endl;
    }
    if (json_obj.contains("data"))
    {
        if (json_obj["data"].contains("server_name"))
        {
            std::string svr_name = json_obj["data"]["server_name"];
            std::cout << "server_name: " << svr_name << std::endl;
        }

        if (json_obj["data"].contains("version"))
        {
            std::string version = json_obj["data"]["version"];
            std::cout << "version: " << version << std::endl;
        }
    }
}

int main(int argc, char** argv)
{

    create_read_write_json();
    read_json_file();

    return 0;
}