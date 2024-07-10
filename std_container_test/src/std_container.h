#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include "to_string.h"

// 单元素容器
template <typename T>
std::string get_single_container_print_str(const T& t)
{
    std::string log = "get_single_container_print_str, size: " + std::to_string(t.size());
    log += "\n[\n";
    std::stringstream buff;
    for(auto it = t.cbegin(); it != t.cend(); ++it)
    {
        buff << "  " <<  *it << "\n";
    }
    log += buff.str();
    log += "]";
    return log;
}

// 多元素容器，包含 map、unordered_map
template <typename T>
std::string get_multi_container_print_str(const T& t)
{
    std::string log = "get_multi_container_print_str, size: " + std::to_string(t.size());
    log += "\n[\n";
    std::stringstream buff;
    for(auto it = t.cbegin(); it != t.cend(); ++it)
    {
        buff << "  " << it->first << " => " << it->second << "\n";
    }
    log += buff.str();
    log += "]\n";
    return std::move(log);
}



