#include "to_string.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>

int main()
{
    using namespace zeno;

    // 测试to_string函数
    std::cout << to_string(123) << std::endl;
    std::cout << to_string(123.456) << std::endl;
    std::cout << to_string(true) << std::endl;
    std::cout << to_string("hello world") << std::endl;

    // 测试to_string函数的泛化版本
    std::cout << to_string(std::vector<int>{1, 2, 3}) << std::endl;
    std::cout << to_string(std::vector<std::string>{"hello", "world"}) << std::endl;

    std::cout << to_string(std::set<int>{6, 1, 2, 3, 4, 5}) << std::endl;
    std::cout << to_string(std::set<std::string>{"hello", "world"}) << std::endl;

    std::cout << to_string(std::map<std::string, int>{{"one", 1}, {"two", 2}, {"three", 3}}) << std::endl;
    std::cout << to_string(std::map<std::string, std::string>{{"hello", "world"}, {"foo", "bar"}}) << std::endl;

    return 0;
}