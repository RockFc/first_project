#include <iostream>
#include <queue>
#include <string>
#include <vector>

struct Item
{
    int         weight;
    std::string name;

    bool operator<(const Item& other) const
    {
        return weight < other.weight;  // 按权重降序排序
    }
};

int main()
{
    std::priority_queue<Item> pq;

    pq.push({10, "Low Priority"});
    pq.push({30, "High Priority"});
    pq.push({20, "Medium Priority-1"});
    pq.push({20, "Medium Priority-2"});

    while (!pq.empty())
    {
        const auto& item = pq.top();
        std::cout << item.weight << " " << item.name << "\n";
        pq.pop();
    }

    return 0;
}