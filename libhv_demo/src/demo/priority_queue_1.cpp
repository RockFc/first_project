#include "hv/hv.h"
#include <iostream>
#include <queue>
#include <string>
#include <vector>

#include "hv/EventLoopThread.h"

using namespace hv;

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
    EventLoopThread     loop_thread;
    const EventLoopPtr& loop = loop_thread.loop();

    std::priority_queue<Item> pq;

    pq.push({10, "Low Priority"});
    pq.push({30, "High Priority"});
    pq.push({20, "Medium Priority-1"});
    pq.push({20, "Medium Priority-2"});

    loop->setTimer(100,
                   [&pq, &loop](TimerID)
                   {
                       if (!pq.empty())
                       {
                           const auto& item = pq.top();
                           loop->queueInLoop(
                               [item]()
                               {
                                   std::cout << item.weight << " " << item.name << " "
                                             << hv_gettid() << std::endl;
                               });
                           pq.pop();
                       }
                   });

    loop_thread.start();
    loop_thread.join();
    return 0;
}