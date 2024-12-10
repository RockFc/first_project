#include "hv/hv.h"
#include <iostream>
#include <queue>
#include <string>
#include <vector>

#include "hv/EventLoopThread.h"

using namespace hv;

struct Item
{
    int                   weight;
    std::string           name;
    std::function<void()> tssk;

    bool operator<(const Item& other) const
    {
        return weight < other.weight;  // 按权重降序排序，即权重越大优先级越高
    }
};

int main()
{
    EventLoopThread     loop_thread;
    const EventLoopPtr& loop = loop_thread.loop();

    std::priority_queue<Item> pq;

    pq.push({10, "Low Priority",
             []()
             {
                 std::cout << "Low Priority Task" << std::endl;
             }});
    pq.push({30, "High Priority",
             []()
             {
                 std::cout << "High Priority Task" << std::endl;
             }});
    pq.push({20, "Medium Priority-1",
             []()
             {
                 std::cout << "Medium Priority-1 Task" << std::endl;
             }});
    pq.push({20, "Medium Priority-2",
             []()
             {
                 std::cout << "Medium Priority-2 Task" << std::endl;
             }});

    loop->setTimer(100,
                   [&pq, &loop](TimerID)
                   {
                       if (!pq.empty())
                       {
                           const auto& item = pq.top();
                           loop->queueInLoop(
                               [item]()
                               {
                                   //    std::cout << item.weight << " " << item.name << " "
                                   //              << hv_gettid() << std::endl;
                                   item.tssk();
                               });
                           pq.pop();
                       }
                   });

    loop_thread.start();
    loop_thread.join();
    return 0;
}