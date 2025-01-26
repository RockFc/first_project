#include <iostream>
#include <map>
#include <tuple>

int main()
{
    std::map<std::string, std::tuple<int, int>> my_map_have_a_long_name;
    std::map<std::tuple<int, int>, std::string> my_map_r;
    my_map_have_a_long_name = {{"aaaaaaa", {1, 2}},
                               {"bbbbbbb", {3, 4}},
                               {"ccccccc", {5, 6}},
                               {"ddddddd", {7, 8}},
                               {"eeeeeee", {9, 10}}};
    for (auto& [key, value] : my_map_have_a_long_name)
    {
        std::cout << key << ": " << std::get<0>(value) << ", " << std::get<1>(value) << std::endl;
    }

    for (auto& [key, value] : my_map_have_a_long_name)
    {
        my_map_r[value] = key;
    }

    for (auto& [key, value] : my_map_r)
    {
        std::cout << std::get<0>(key) << ", " << std::get<1>(key) << ": " << value << std::endl;
    }

    return 0;
}